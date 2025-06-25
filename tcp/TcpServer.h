#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include "GXDLMSServer.h"
class DlmsHandler;

class TcpServer {
public:
    TcpServer(int port, DlmsHandler* handler);
    ~TcpServer();

    void Start();
    void Stop();
    CGXDLMSServer* GetDlmsServer() { return m_server; }
private:
    void Run();
    CGXDLMSServer* m_server;

    int m_Port;
    DlmsHandler* m_Handler;
    std::thread m_Thread;
    std::atomic<bool> m_Running;
};