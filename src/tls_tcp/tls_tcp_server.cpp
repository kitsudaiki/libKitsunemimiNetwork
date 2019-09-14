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
#include <logger/logger.h>

using namespace Kitsune::Persistence;

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
    uint32_t length = sizeof(struct sockaddr_in);

    //make new connection
    int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);
    if(fd < 0)
    {
        LOG_error("Failed accept incoming connection on encrypted tcp-server with "
                  "port: " + std::to_string(m_port));
        return nullptr;
    }

    // create new socket-object from file-descriptor
    TlsTcpSocket* tcpSocket = new TlsTcpSocket(fd,
                                               m_certFile,
                                               m_keyFile);
    if(tcpSocket->initOpenssl() == false)
    {
        delete tcpSocket;
        return nullptr;
    }

    LOG_info("Successfully accepted incoming connection on encrypted tcp-socket server with "
             "port : " + std::to_string(m_port));

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
