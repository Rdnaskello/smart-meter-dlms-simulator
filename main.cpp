    #include "cli/CLI.h"
    #include "core/DlmsHandler.h"
    #include "model/ObisObjectModel.h"
    #include "tcp/TcpServer.h"
    #include "updater/Updater.h"
    #include "profile/ProfileGenericManager.h"
    #include <vector>
    #include <memory>
    #include <iostream>
    #include <functional>


    double GetEnergyValue(CGXDLMSRegister* reg) {
    return reg ? reg->GetValue().ToDouble() : 0.0;
    }

    int main(int argc, char* argv[]) {
        Config config = ParseArguments(argc, argv);

        std::vector<std::unique_ptr<TcpServer>> servers;
        std::vector<std::unique_ptr<DlmsHandler>> handlers;
        std::vector<std::unique_ptr<DataUpdater>> updaters;
        std::vector<std::unique_ptr<ProfileGenericManager>> profileManagers;

        for (int i = 0; i < config.count; ++i) {
            int port = config.startPort + i;
            unsigned long serialNumber = 100000000 + i;

            auto association = new CGXDLMSAssociationLogicalName("0.0.40.0.0.255");
            auto tcpUdpSetup = new CGXDLMSTcpUdpSetup();
            auto handler = std::make_unique<DlmsHandler>(association, tcpUdpSetup, nullptr, port);

            ObisObjectModel model;
            model.AddBasicTestObisObjects(handler->GetItems(), serialNumber);

            CGXDLMSClock* clock = model.GetClock();
            CGXDLMSRegister* energyRegister = model.GetEnergyRegister();
            std::cout << "[DEBUG] clock: " << clock << ", energyRegister: " << energyRegister << std::endl;

            std::string profileFile = "../data/profile" + std::to_string(port) + ".csv";
            auto profileManager = std::make_unique<ProfileGenericManager>(profileFile);

            auto* profileGeneric = profileManager->CreateProfileGeneric(clock, energyRegister);
            handler->GetItems().push_back(profileGeneric);

            auto getEnergyValue = [energyRegister]() {
                return energyRegister ? energyRegister->GetValue().ToDouble() : 0.0;
            };

            // Додаємо запуск потоку генерації даних
            profileManager->StartPeriodicUpdate(getEnergyValue);

            for (auto& pair : profileGeneric->GetCaptureObjects()) {
                CGXDLMSObject* obj = pair.first;
                std::string ln;
                obj->GetLogicalName(ln);
                std::cout << "[DEBUG] CaptureObject: " << ln << std::endl;

                bool found = false;
                for (auto& it : handler->GetItems()) {
                    std::string ln2;
                    it->GetLogicalName(ln2);
                    if (ln == ln2) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "❌ [ERROR] Object " << ln << " not found in items!" << std::endl;
                }
            }

            auto updater = std::make_unique<DataUpdater>(handler->GetItems(), i + 1);

            handler->Initialize();

            auto server = std::make_unique<TcpServer>(port, handler.get());
            
            server->Start();
            updater->Start();

            servers.push_back(std::move(server));
            handlers.push_back(std::move(handler));
            updaters.push_back(std::move(updater));
            profileManagers.push_back(std::move(profileManager)); // <-- додали сюди

            std::cout << "✅ Meter " << (i + 1) << " started on port " << port << std::endl;
            std::cout << "   ./smart_meter_client -h localhost -p " << port << " -i WRAPPER -w 1 -c 16 -s 1 -r LN\n";
        }

        std::cout << "\nAll meters are running. Press Enter to stop...\n";
        std::cin.get();

        for (auto& updater : updaters) {
            updater->Stop();
        }

        for (auto& server : servers) {
            server->Stop();
        }

        return 0;
    }
