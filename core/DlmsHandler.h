#pragma once

#include <vector>
#include <memory>
#include "GXDLMSSecureServer.h"
#include "GXDLMSSecuritySetup.h"
#include "../updater/Updater.h"
#include "../profile/ProfileGenericManager.h"  // Додай підключення!

class DlmsHandler : public CGXDLMSSecureServer {
public:
    DlmsHandler(CGXDLMSAssociationLogicalName* ln, CGXDLMSIecHdlcSetup* hdlc, DataUpdater* updater, int port);
    DlmsHandler(CGXDLMSAssociationLogicalName* ln, CGXDLMSTcpUdpSetup* wrapper, DataUpdater* updater, int port);
    DlmsHandler(CGXDLMSAssociationShortName* sn, CGXDLMSIecHdlcSetup* hdlc, DataUpdater* updater, int port);
    DlmsHandler(CGXDLMSAssociationShortName* sn, CGXDLMSTcpUdpSetup* wrapper, DataUpdater* updater, int port);

    ~DlmsHandler();

    void HandleRequest(const std::vector<unsigned char>& request, std::vector<unsigned char>& response);
    int Initialize();

    void PreRead(std::vector<CGXDLMSValueEventArg*>& args);
    void PostRead(std::vector<CGXDLMSValueEventArg*>& args);
    void PreWrite(std::vector<CGXDLMSValueEventArg*>& args);
    void PostWrite(std::vector<CGXDLMSValueEventArg*>& args);
    void PreAction(std::vector<CGXDLMSValueEventArg*>& args);
    void PostAction(std::vector<CGXDLMSValueEventArg*>& args);
    void PreGet(std::vector<CGXDLMSValueEventArg*>& args);
    void PostGet(std::vector<CGXDLMSValueEventArg*>& args);

    DLMS_SOURCE_DIAGNOSTIC ValidateAuthentication(DLMS_AUTHENTICATION authentication, CGXByteBuffer& password);
    DLMS_ACCESS_MODE GetAttributeAccess(CGXDLMSValueEventArg* arg);
    DLMS_METHOD_ACCESS_MODE GetMethodAccess(CGXDLMSValueEventArg* arg);
    CGXDLMSObject* FindObject(DLMS_OBJECT_TYPE objectType, int sn, std::string& ln) override;

    void Connected(CGXDLMSConnectionEventArgs& connectionInfo);
    void InvalidConnection(CGXDLMSConnectionEventArgs& connectionInfo);
    void Disconnected(CGXDLMSConnectionEventArgs& connectionInfo);

    bool IsTarget(unsigned long serverAddress, unsigned long clientAddress);
    void GetIndicesByTimeRange(CGXDateTime& start, CGXDateTime& end, int& rowBegin, int& rowEnd);

    void SetProfileManager(ProfileGenericManager* profileManager) { m_ProfileManager = profileManager; }

private:
    void InitializeServer();
    DataUpdater* m_Updater = nullptr;
    ProfileGenericManager* m_ProfileManager = nullptr; 
    int m_Port = 0;
};
