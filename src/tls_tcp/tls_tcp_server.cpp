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
