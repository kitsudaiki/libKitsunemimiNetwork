/**
 *  @file    tcp_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsuneNetwork/tcp/tcp_server.h>
#include <libKitsuneNetwork/tcp/tcp_socket.h>
#include <libKitsunePersistence/logger/logger.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
TcpServer::TcpServer(void* target,
                     void (*processConnection)(void*, AbstractSocket*))
    : AbstractServer(target,
                     processConnection)
{
    m_type = TCP_SERVER;
}

/**
 * destructor
 */
TcpServer::~TcpServer()
{
    closeServer();
}

/**
 * creates a server on a specific port
 *
 * @param port port-number where the server should be listen
 *
 * @return false, if server creation failed, else true
 */
bool
TcpServer::initServer(const uint16_t port)
{
    m_port = port;

    // create socket
    m_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_serverSocket < 0)
    {
        KS::LOG_error("Failed to create a tcp-socket");
        return false;
    }

    // make the port reusable
    int enable = 1;
    if(setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        KS::LOG_error("Failed set socket-options for tcp-server on port: " + std::to_string(m_port));
        return false;
    }

    // set server-settings
    memset(&m_server, 0, sizeof (m_server));
    m_server.sin_family = AF_INET;
    m_server.sin_addr.s_addr = htonl(INADDR_ANY);
    m_server.sin_port = htons(m_port);

    // bind to port
    if(bind(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), sizeof(m_server)) < 0)
    {
        KS::LOG_error("Failed to bind tcp-socket to port: " + std::to_string(m_port));
        return false;
    }

    // start listening for incoming connections
    if(listen(m_serverSocket, 5) == -1)
    {
        KS::LOG_error("Failed listen on tcp-socket on port: " + std::to_string(m_port));
        return false;
    }

    KS::LOG_info("Successfully initialized tcp-socket server on port: " + std::to_string(m_port));

    return true;
}

/**
 * wait for new incoming tcp-connections
 *
 * @return new tcp-socket-socket-pointer for the new incoming connection
 */
AbstractSocket* TcpServer::waitForIncomingConnection()
{
    uint32_t length = sizeof(struct sockaddr_in);

    //make new connection
    int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);

    if(m_abort) {
        return nullptr;
    }

    if(fd < 0)
    {
        KS::LOG_error("Failed accept incoming connection on tcp-server with "
                      "port: " + std::to_string(m_port));
        return nullptr;
    }

    KS::LOG_info("Successfully accepted incoming connection on tcp-socket server with "
                 "port : " + std::to_string(m_port));

    // create new socket-object from file-descriptor
    TcpSocket* tcpSocket = new TcpSocket(fd);
    m_processConnection(m_target, tcpSocket);

    // append new socket to the list
    mutexLock();
    m_sockets.push_back(tcpSocket);
    mutexUnlock();

    return tcpSocket;
}

} // namespace Network
} // namespace Kitsune
