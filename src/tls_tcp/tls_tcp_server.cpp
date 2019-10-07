/**
 *  @file    tls_tcp_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsuneNetwork/tls_tcp/tls_tcp_server.h>
#include <libKitsuneNetwork/tls_tcp/tls_tcp_socket.h>
#include <libKitsunePersistence/logger/logger.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
TlsTcpServer::TlsTcpServer(void* target,
                           void (*processConnection)(void*, AbstractSocket*),
                           const std::string certFile,
                           const std::string keyFile,
                           const std::string caFile)
    : TcpServer(target,
                processConnection)
{
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_caFile = caFile;

    m_type = TLS_TCP_SERVER;
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

    if(m_abort) {
        return nullptr;
    }

    if(fd < 0)
    {
        KS::LOG_error("Failed accept incoming connection on encrypted tcp-server with "
                      "port: " + std::to_string(m_port));
        return nullptr;
    }

    // create new socket-object from file-descriptor
    TlsTcpSocket* tcpSocket = new TlsTcpSocket(fd,
                                               m_certFile,
                                               m_keyFile,
                                               m_caFile);
    if(tcpSocket->initOpenssl() == false)
    {
        delete tcpSocket;
        return nullptr;
    }

    KS::LOG_info("Successfully accepted incoming connection on encrypted tcp-socket server with "
                 "port : " + std::to_string(m_port));

    m_processConnection(m_target, tcpSocket);

    // append new socket to the list
    mutexLock();
    m_sockets.push_back(tcpSocket);
    mutexUnlock();

    return tcpSocket;
}

} // namespace Network
} // namespace Kitsune
