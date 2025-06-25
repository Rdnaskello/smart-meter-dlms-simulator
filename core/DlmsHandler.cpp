#include "DlmsHandler.h"
#include "GXDLMSSettings.h"
#include "GXBytebuffer.h"
#include "../updater/Updater.h"
#include "../model/ObisObjectModel.h"
#include <iostream>

// Конструктори
DlmsHandler::DlmsHandler(CGXDLMSAssociationLogicalName* ln, CGXDLMSIecHdlcSetup* hdlc, DataUpdater* updater, int port)
    : CGXDLMSSecureServer(ln, hdlc), m_Updater(updater), m_Port(port) {
    InitializeServer();
}

DlmsHandler::DlmsHandler(CGXDLMSAssociationLogicalName* ln, CGXDLMSTcpUdpSetup* wrapper, DataUpdater* updater, int port)
    : CGXDLMSSecureServer(ln, wrapper), m_Updater(updater), m_Port(port) {
    InitializeServer();
}

DlmsHandler::DlmsHandler(CGXDLMSAssociationShortName* sn, CGXDLMSIecHdlcSetup* hdlc, DataUpdater* updater, int port)
    : CGXDLMSSecureServer(sn, hdlc), m_Updater(updater), m_Port(port) {
    InitializeServer();
}

DlmsHandler::DlmsHandler(CGXDLMSAssociationShortName* sn, CGXDLMSTcpUdpSetup* wrapper, DataUpdater* updater, int port)
    : CGXDLMSSecureServer(sn, wrapper), m_Updater(updater), m_Port(port) {
    InitializeServer();
}

DlmsHandler::~DlmsHandler() {}

void DlmsHandler::InitializeServer() {
    SetUseLogicalNameReferencing(true);
    GetSettings().SetInterfaceType(DLMS_INTERFACE_TYPE_WRAPPER);
    SetMaxReceivePDUSize(1024);
}

void DlmsHandler::HandleRequest(const std::vector<unsigned char>& request, std::vector<unsigned char>& response) {
    CGXByteBuffer reqBuf;
    reqBuf.Set(request.data(), (int)request.size());
    
    CGXByteBuffer resBuf;
    int ret = CGXDLMSSecureServer::HandleRequest(reqBuf, resBuf);
    if (ret == 0 && resBuf.GetSize() > 0) {
        response.assign(resBuf.GetData(), resBuf.GetData() + resBuf.GetSize());
    }

    //std::cout << "🔵 HandleRequest processed. Response size: " << response.size() << std::endl;
}

int DlmsHandler::Initialize() {
    int ret = CGXDLMSSecureServer::Initialize();

    std::cout << "🛠️ DLMS сервер ініціалізовано\n";

    if (ret != 0) {
        std::cerr << "❌ Failed to initialize DLMS server. Error code: " << ret << std::endl;
    } else {
        std::cout << "✅ DLMS server initialized successfully." << std::endl;
    }

    return ret;
}

void DlmsHandler::PreRead(std::vector<CGXDLMSValueEventArg*>& args) {
    std::cout << "✅ [PreRead] Функція викликана!" << std::endl;
    for (auto& arg : args) {
        CGXDLMSObject* target = arg->GetTarget();
        if (!target) continue;

        auto objectType = target->GetObjectType();
        int index = arg->GetIndex();

        if (objectType == DLMS_OBJECT_TYPE_PROFILE_GENERIC && index == 2) {
            auto* profile = static_cast<CGXDLMSProfileGeneric*>(target);
            if (m_ProfileManager) {
                int selector = arg->GetSelector();
                std::cout << "\n[PreRead] ======== Запит на ProfileGeneric.buffer ========" << std::endl;
                std::cout << "[PreRead] Selector: " << selector << std::endl;

                int rowBegin = 1;
                int rowEnd = m_ProfileManager->GetDataCount();

                if (selector == 1) {
                    if (arg->GetParameters().Arr.size() >= 2) {
                        CGXByteBuffer bb;
                        CGXDLMSVariant start, end;

                        bb.Set(arg->GetParameters().Arr[0].byteArr, arg->GetParameters().Arr[0].size);
                        CGXDLMSClient::ChangeType(bb, DLMS_DATA_TYPE_DATETIME, start);

                        bb.Clear();
                        bb.Set(arg->GetParameters().Arr[1].byteArr, arg->GetParameters().Arr[1].size);
                        CGXDLMSClient::ChangeType(bb, DLMS_DATA_TYPE_DATETIME, end);

                        CGXDateTime startDT = start.dateTime;
                        CGXDateTime endDT = end.dateTime;

                        m_ProfileManager->GetIndicesByTimeRange(startDT, endDT, rowBegin, rowEnd);
                        std::cout << "[PreRead] Selector=1. RowBegin=" << rowBegin << ", RowEnd=" << rowEnd << std::endl;
                    }
                }
                else if (selector == 2) {
                    auto params = arg->GetParameters().Arr;
                    if (params.size() >= 2) {
                        rowBegin = params[0].ToInteger();
                        rowEnd = params[1].ToInteger();
                        std::cout << "[PreRead] Selector=2. RowBegin=" << rowBegin << ", RowEnd=" << rowEnd << std::endl;
                    }
                }
                else {
                    std::cout << "[PreRead] Selector=0 або невідомий. Читаємо весь буфер." << std::endl;
                }

                arg->SetRowBeginIndex(rowBegin);
                arg->SetRowEndIndex(rowEnd);

                // Завантажуємо дані з файлу
                m_ProfileManager->LoadFromFile(profile);
                profile->SetEntriesInUse(m_ProfileManager->GetDataCount());
                profile->SetProfileEntries(m_ProfileManager->GetDataCount());

                std::cout << "[PreRead] Завантажено даних з файлу: " << profile->GetBuffer().size() << " рядків." << std::endl;

                int rowNum = 1;
                for (const auto& row : profile->GetBuffer()) {
                    std::cout << "  [File] Рядок #" << rowNum++ << ": ";
                    for (auto val : row) {  // <-- Використовуємо копію, щоб працювало ToString()
                        std::cout << val.ToString() << " | ";
                    }
                    std::cout << std::endl;
                }

                // Формуємо CGXDLMSVariant для відповіді
                CGXDLMSVariant bufferValue;
                bufferValue.vt = DLMS_DATA_TYPE_ARRAY;

                int currentRow = 1;
                for (const auto& row : profile->GetBuffer()) {
                    if (currentRow >= rowBegin && currentRow <= rowEnd) {
                        CGXDLMSVariant rowVariant;
                        rowVariant.vt = DLMS_DATA_TYPE_ARRAY;
                        rowVariant.Arr = row;
                        bufferValue.Arr.push_back(rowVariant);

                        std::cout << "  [PreRead] Додано рядок #" << currentRow << " для клієнта: ";
                        for (auto val : row) {
                            std::cout << val.ToString() << " | ";
                        }
                        std::cout << std::endl;
                    }
                    currentRow++;
                }

                std::cout << "[PreRead] Перед відправкою клієнту. Кількість рядків: " << bufferValue.Arr.size() << std::endl;
                arg->SetValue(bufferValue);
                arg->SetHandled(true);

                std::cout << "====================================================\n" << std::endl;
                continue;
            }
        }

        if (objectType == DLMS_OBJECT_TYPE_CLOCK && index == 2) {
            auto* clk = static_cast<CGXDLMSClock*>(target);
            CGXDateTime now = CGXDateTime::Now();
            DATETIME_SKIPS skip = clk->GetTime().GetSkip();
            now.SetSkip(skip);
            clk->SetTime(now);
            arg->SetValue(now);
            arg->SetHandled(true);
            std::cout << "[PreRead] Clock оновлено: " << now.ToString() << std::endl;
            continue;
        }

        if (objectType == DLMS_OBJECT_TYPE_REGISTER && index == 2) {
            std::string ln;
            target->GetLogicalName(ln);
            double value = 0.0;
            if (m_Updater && m_Updater->GetValue(ln, value)) {
                arg->SetValue(value);
                arg->SetHandled(true);
                std::cout << "[PreRead] Register " << ln << " = " << value << std::endl;
            }
            continue;
        }
    }
}




void DlmsHandler::PreGet(std::vector<CGXDLMSValueEventArg*>& args) {
    for (auto& arg : args) {
        CGXDLMSObject* target = arg->GetTarget();
        if (!target) continue;

        if (target->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC) {
            auto* profile = static_cast<CGXDLMSProfileGeneric*>(target);
            std::cout << "[PreGet] Capture triggered for ProfileGeneric" << std::endl;
            if (m_ProfileManager) {
                m_ProfileManager->LoadFromFile(profile);
            }
            arg->SetHandled(true);
        }
    }
}


void DlmsHandler::PostRead(std::vector<CGXDLMSValueEventArg*>& args) {}
void DlmsHandler::PreWrite(std::vector<CGXDLMSValueEventArg*>& args) {}
void DlmsHandler::PostWrite(std::vector<CGXDLMSValueEventArg*>& args) {}
void DlmsHandler::PreAction(std::vector<CGXDLMSValueEventArg*>& args) {}

void DlmsHandler::PostAction(std::vector<CGXDLMSValueEventArg*>& args) {
    for (auto& arg : args) {
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC && arg->GetIndex() == 1) {
            if (m_ProfileManager) {
                m_ProfileManager->Clear();
                std::cout << "[DlmsHandler] 📂 Профіль очищено (Clear)." << std::endl;
            }
        }
    }
}

void DlmsHandler::PostGet(std::vector<CGXDLMSValueEventArg*>& args) {}

DLMS_SOURCE_DIAGNOSTIC DlmsHandler::ValidateAuthentication(DLMS_AUTHENTICATION authentication, CGXByteBuffer& password) {
    return DLMS_SOURCE_DIAGNOSTIC_NONE;
}

DLMS_ACCESS_MODE DlmsHandler::GetAttributeAccess(CGXDLMSValueEventArg* arg) {
    return DLMS_ACCESS_MODE_READ;
}

DLMS_METHOD_ACCESS_MODE DlmsHandler::GetMethodAccess(CGXDLMSValueEventArg* arg) {
    return DLMS_METHOD_ACCESS_MODE_ACCESS;
}

void DlmsHandler::Connected(CGXDLMSConnectionEventArgs& connectionInfo) {
    std::cout << "🟢 Connected" << std::endl;
}

void DlmsHandler::InvalidConnection(CGXDLMSConnectionEventArgs& connectionInfo) {
    std::cout << "❌ InvalidConnection" << std::endl;
}

void DlmsHandler::Disconnected(CGXDLMSConnectionEventArgs& connectionInfo) {
    std::cout << "🔴 Disconnected" << std::endl;
}

CGXDLMSObject* DlmsHandler::FindObject(DLMS_OBJECT_TYPE objectType, int sn, std::string& ln) {
    for (auto& obj : GetItems()) {
        std::string currentLn;
        obj->GetLogicalName(currentLn);
        if (currentLn == ln) {
            return obj;
        }
    }
    return nullptr;
}


bool DlmsHandler::IsTarget(unsigned long serverAddress, unsigned long clientAddress) {
    return true;
}