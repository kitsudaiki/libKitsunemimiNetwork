/**
 *  @file    tls_tcp_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_server.h>
#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_socket.h>
#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor
 */
TlsTcpServer::TlsTcpServer(void* target,
                           void (*processConnection)(void*, AbstractSocket*),
                           const std::string &threadName,
                           const std::string &certFile,
                           const std::string &keyFile,
                           const std::string &caFile)
    : TcpServer(target,
                processConnection,
                threadName)
{
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_caFile = caFile;

    m_type = TLS_TCP_SERVER;
}

/**
 * @brief destructor
 */
TlsTcpServer::~TlsTcpServer()
{
    closeServer();
}

/**
 * @brief wait for new incoming tcp-connections
 *
 * @param error reference for error-output
 */
bool
TlsTcpServer::waitForIncomingConnection(ErrorContainer &error)
{
    uint32_t length = sizeof(struct sockaddr_in);

    //make new connection
    const int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);

    if(m_abort) {
        return true;
    }

    if(fd < 0)
    {
        error.addMeesage("Failed accept incoming connection on tcp-server with port: "
                         + std::to_string(m_port));
        return false;
    }

    // create new socket-object from file-descriptor
    const std::string name = getThreadName() + "_client";
    TlsTcpSocket* tcpSocket = new TlsTcpSocket(fd,
                                               name,
                                               m_certFile,
                                               m_keyFile,
                                               m_caFile);
    if(tcpSocket->initOpenssl(error) == false)
    {
        delete tcpSocket;
        return false;
    }

    LOG_INFO("Successfully accepted incoming connection on encrypted tcp-socket server with "
             "port : " + std::to_string(m_port));

    m_processConnection(m_target, tcpSocket);

    return true;
}

} // namespace Network
} // namespace Kitsunemimi
