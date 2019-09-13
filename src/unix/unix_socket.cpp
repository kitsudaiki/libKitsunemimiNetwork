/**
 *  @file    unix_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <unix/unix_socket.h>
#include <iostream>
#include <network_trigger.h>
#include <cleanup_thread.h>

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
UnixSocket::UnixSocket(const std::string socketFile)
    : AbstractSocket ()
{
    m_socketFile = socketFile;
    m_clientSide = true;
}

/**
 * constructor for the server-side of the unix-socket-connection, which is called by the
 * unix-server for each incoming connection
 *
 * @param socketFd file-descriptor of the socket-socket
 */
UnixSocket::UnixSocket(const int socketFd)
    : AbstractSocket ()
{
    m_socket = socketFd;
    m_clientSide = false;
}

/**
 * init unix-socket and connect to the server
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
UnixSocket::initSocket()
{
    struct sockaddr_un address;

    // create socket
    m_socket = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(m_socket < 0) {
        return false;
    }

    // set other informations
    address.sun_family = AF_LOCAL;
    strncpy(address.sun_path, m_socketFile.c_str(), m_socketFile.size());

    // create connection
    if(connect(m_socket, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0)
    {
        // TODO: correctly close socket
        m_socket = 0;
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
UnixSocket::recvData(int socket,
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
UnixSocket::sendData(int socket,
                     const void* bufferPosition,
                     const size_t bufferSize,
                     int flags)
{
    return send(socket, bufferPosition, bufferSize, flags);
}

} // namespace Network
} // namespace Kitsune
