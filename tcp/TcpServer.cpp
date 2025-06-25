// TcpServer.cpp
#include "TcpServer.h"
#include <iostream>
#include <cstring>
#ifndef _WIN32
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
#else
    #include <winsock2.h>
    typedef int socklen_t;
#endif

#include "../core/DlmsHandler.h"

TcpServer::TcpServer(int port, DlmsHandler* handler)
    : m_Port(port), m_Handler(handler), m_Running(false), m_server(handler) {
}

TcpServer::~TcpServer() {
    Stop();
}

void TcpServer::Start() {
    m_Running = true;
    m_Thread = std::thread(&TcpServer::Run, this);
}

void TcpServer::Stop() {
    m_Running = false;
    if (m_Thread.joinable()) {
        m_Thread.join();
    }
}

void TcpServer::Run() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "❌ Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(m_Port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "❌ Bind failed" << std::endl;
        return;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "❌ Listen failed" << std::endl;
        return;
    }

    std::cout << "TCP Server started on port " << m_Port << std::endl;

    while (m_Running) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket == INVALID_SOCKET) {
            if (m_Running) {
                std::cerr << "❌ Accept failed" << std::endl;
            }
            continue;
        }

        std::thread([this, clientSocket]() {
            char buffer[1024];
            int bytesRead;

            while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
                std::vector<unsigned char> request(buffer, buffer + bytesRead);
                std::vector<unsigned char> response;

                m_Handler->HandleRequest(request, response);

                if (!response.empty()) {
                    send(clientSocket, reinterpret_cast<const char*>(response.data()), response.size(), 0);
                }
            }

#ifdef _WIN32
            closesocket(clientSocket);
#else
            close(clientSocket);
#endif
            std::cout << "🔴 Client disconnected" << std::endl;
        }).detach();
    }

#ifdef _WIN32
    closesocket(serverSocket);
#else
    close(serverSocket);
#endif

    std::cout << "TCP Server stopped" << std::endl;
}
