#include "Updater.h"
#include <iostream>
#include <chrono>
#include <random>
#include "GXDLMSRegister.h"

DataUpdater::DataUpdater(CGXDLMSObjectCollection& items, int id) : m_Items(items), m_Id(id), m_Running(false) {}

DataUpdater::~DataUpdater() {
    Stop();
}

void DataUpdater::Start() {
    m_Running = true;
    m_Thread = std::thread(&DataUpdater::Run, this);
}

void DataUpdater::Stop() {
    m_Running = false;
    if (m_Thread.joinable()) {
        m_Thread.join();
    }
}

void DataUpdater::Run() {
    while (m_Running) {
        for (auto& obj : m_Items) {
            if (obj->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER) {
                auto* reg = dynamic_cast<CGXDLMSRegister*>(obj);
                if (reg) {
                    std::string ln;
                    reg->GetLogicalName(ln);

                    double newValue = 0.0;
                    if (ln == "1.0.21.8.0.255" || ln == "1.0.41.8.0.255" || ln == "1.0.61.8.0.255") {
                        m_Values[ln] += 0.01;
                        newValue = m_Values[ln];
                    }
                    else if (ln == "1.0.1.8.0.255") {
                        newValue = m_Values["1.0.21.8.0.255"] + m_Values["1.0.41.8.0.255"] + m_Values["1.0.61.8.0.255"];
                        m_Values[ln] = newValue;
                    }

                    CGXDLMSVariant variant;
                    variant = newValue;
                    reg->SetValue(variant);

                    //std::cout << "🔄 [Meter " << m_Id << "] Updated " << ln << " = " << newValue << std::endl;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}




bool DataUpdater::GetValue(const std::string& obis, double& value) {
    auto it = m_Values.find(obis);
    if (it != m_Values.end()) {
        value = it->second;
        return true;
    }
    return false;
}

