#include <tcp/tcpServer.h>
#include <tcp/tcpClient.h>
#include <iostream>

namespace Kitsune
{
namespace Network
{

/**
 * @brief TcpServer::TcpServer
 */
TcpServer::TcpServer()
{

}

/**
 * @brief TcpServer::~TcpServer
 */
TcpServer::~TcpServer()
{
    closeServer();
}

/**
 * @brief Kitsune::Network::TcpServer::initSocket
 * @param port
 * @return
 */
bool TcpServer::initSocket(const uint16_t port)
{
    // create socket
    m_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_serverSocket < 0) {
        return false;
    }
    // make the port reusable
    int enable = 1;
    if(setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        return false;
    }

    // set server-settings
    memset(&m_server, 0, sizeof (m_server));
    m_server.sin_family = AF_INET;
    m_server.sin_addr.s_addr = htonl(INADDR_ANY);
    m_server.sin_port = htons(port);

    // bind to port
    if(bind(m_serverSocket, (struct sockaddr*)&m_server, sizeof(m_server)) < 0) {
        return false;
    }

    // start listening for incoming connections
    if(listen(m_serverSocket, 5) == -1) {
        return false;
    }

    return true;
}

/**
 * @brief Kitsune::Network::TcpServer::waitForMessage
 * @return
 */
TcpClient* TcpServer::waitForIncomingConnection()
{
    struct sockaddr_in client;
    uint32_t length = sizeof(client);

    //make new connection
    int fd = accept(m_serverSocket, (struct sockaddr*)&client, &length);
    if(fd < 0) {
        return nullptr;
    }

    // create new client-object from file-descriptor
    TcpClient* tcpClient = new TcpClient(fd, client);
    mutexLock();
    m_sockets.push_back(tcpClient);
    mutexUnlock();

    return tcpClient;
}

/**
 * @brief Kitsune::Network::TcpServer::closeSocket
 * @return
 */
bool TcpServer::closeServer()
{
    if(m_serverSocket >= 0)
    {
        // close all connected sockets
        mutexLock();
        for(uint32_t i = 0; i < m_sockets.size(); i++) {
            m_sockets[i]->closeSocket();
        }
        m_sockets.clear();
        mutexUnlock();

        // close server itself
        close(m_serverSocket);
        m_serverSocket = 0;
        return true;
    }
    return false;
}

/**
 * @brief TcpServer::getNumberOfSockets
 * @return
 */
uint32_t TcpServer::getNumberOfSockets()
{
    uint32_t result = 0;
    mutexLock();
    result = m_sockets.size();
    mutexUnlock();
    return result;
}

/**
 * @brief TcpServer::getSocket
 * @param pos
 * @return
 */
TcpClient *TcpServer::getSocket(const uint32_t pos)
{
    TcpClient* result = nullptr;
    mutexLock();
    if(pos < m_sockets.size()) {
        result = m_sockets.at(pos);
    }
    mutexUnlock();
    return result;
}

/**
 * @brief TcpServer::run
 */
void TcpServer::run()
{
    while(!m_abort) {
        waitForIncomingConnection();
    }
}

}
}
