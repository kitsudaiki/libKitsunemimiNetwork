/**
 *  @file    tls_tcp_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <tls_tcp/tls_tcp_server.h>
#include <tls_tcp/tls_tcp_socket.h>
#include <network_trigger.h>
#include <iostream>

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
TlsTcpServer::TlsTcpServer(const std::string certFile,
                           const std::string keyFile,
                           NetworkTrigger* trigger)
{
    m_certFile = certFile;
    m_keyFile = keyFile;

    // BUG: if trigger is nullptr, this is not checked and added to the list
    m_trigger.push_back(trigger);
}

/**
 * destructor
 */
TlsTcpServer::~TlsTcpServer()
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
TlsTcpServer::initServer(const uint16_t port)
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
 * wait for new incoming tcp-connections
 *
 * @return new tcp-socket-socket-pointer for the new incoming connection
 */
AbstractSocket*
TlsTcpServer::waitForIncomingConnection()
{
    struct sockaddr_in socket;
    uint32_t length = sizeof(socket);

    //make new connection
    int fd = accept(m_serverSocket, (struct sockaddr*)&socket, &length);
    if(fd < 0) {
        return nullptr;
    }

    // create new socket-object from file-descriptor
    TlsTcpSocket* tcpSocket = new TlsTcpSocket(fd,
                                               socket,
                                               m_certFile,
                                               m_keyFile);
    for(uint32_t i = 0; i < m_trigger.size(); i++) 
    {
        tcpSocket->addNetworkTrigger(m_trigger.at(i));
    }

    // start new socket-thread
    tcpSocket->start();

    // append new socket to the list
    mutexLock();
    m_sockets.push_back(tcpSocket);
    mutexUnlock();

    return tcpSocket;
}

} // namespace Network
} // namespace Kitsune
