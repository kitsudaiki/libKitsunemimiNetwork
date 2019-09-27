/**
 *  @file    unix_domain_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <unix/unix_domain_socket.h>
#include <iostream>
#include <message_trigger.h>
#include <cleanup_thread.h>
#include <logger/logger.h>

using namespace Kitsune::Persistence;

namespace Kitsune
{
namespace Network
{

/**
 * constructor for the socket-side of the unix-socket-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 */
UnixDomainSocket::UnixDomainSocket(const std::string socketFile)
    : AbstractSocket ()
{
    m_socketFile = socketFile;
    m_clientSide = true;
    m_type = UNIX_SOCKET;
}

/**
 * @brief init socket on client-side
 *
 * @return true, if successful, else false
 */
bool
UnixDomainSocket::initClientSide()
{
    bool result = initSocket();
    if(result == false) {
        return false;
    }

    LOG_info("Successfully initialized unix-socket client to targe: " + m_socketFile);

    return true;
}

/**
 * constructor for the server-side of the unix-socket-connection, which is called by the
 * unix-server for each incoming connection
 *
 * @param socketFd file-descriptor of the socket-socket
 */
UnixDomainSocket::UnixDomainSocket(const int socketFd)
    : AbstractSocket ()
{
    m_socket = socketFd;
    m_clientSide = false;
    m_type = UNIX_SOCKET;
}

/**
 * init unix-socket and connect to the server
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
UnixDomainSocket::initSocket()
{
    struct sockaddr_un address;

    // create socket
    m_socket = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(m_socket < 0)
    {
        LOG_error("Failed to create a unix-socket");
        return false;
    }

    // set other informations
    address.sun_family = AF_LOCAL;
    strncpy(address.sun_path, m_socketFile.c_str(), m_socketFile.size());

    // create connection
    if(connect(m_socket, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0)
    {
        LOG_error("Failed to connect unix-socket to server with addresse: " + m_socketFile);
        return false;
    }

    m_socketAddr = address;

    return true;
}

/**
 * @brief receive data
 *
 * @return number of read bytes
 */
long
UnixDomainSocket::recvData(int socket,
                           void* bufferPosition,
                           const size_t bufferSize,
                           int flags)
{
    return recv(socket, bufferPosition, bufferSize, flags);
}

/**
 * @brief send data
 *
 * @return number of written bytes
 */
ssize_t
UnixDomainSocket::sendData(int socket,
                           const void* bufferPosition,
                           const size_t bufferSize,
                           int flags)
{
    return send(socket, bufferPosition, bufferSize, flags);
}

} // namespace Network
} // namespace Kitsune
