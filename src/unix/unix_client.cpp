/**
 *  @file    unix_client.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <unix/unix_client.h>
#include <iostream>
#include <network_trigger.h>
#include <cleanup_thread.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor for the client-side of the unix-socket-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 */
UnixClient::UnixClient(const std::string socketFile)
    : AbstractClient ()
{
    m_socketFile = socketFile;
    m_clientSide = true;
    initClientSide();
}

/**
 * constructor for the server-side of the unix-socket-connection, which is called by the
 * unix-server for each incoming connection
 *
 * @param clientFd file-descriptor of the client-socket
 * @param client address for the client
 */
UnixClient::UnixClient(const int clientFd, sockaddr_un client)
    : AbstractClient ()
{
    m_clientSocket = clientFd;
    m_client = client;
    m_clientSide = false;
}

/**
 * init unix-socket and connect to the server
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
UnixClient::initClientSide()
{
    struct sockaddr_un address;

    // create socket
    m_clientSocket = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(m_clientSocket < 0) {
        return false;
    }

    // set other informations
    address.sun_family = AF_LOCAL;
    strncpy(address.sun_path, m_socketFile.c_str(), m_socketFile.size());

    // create connection
    if(connect(m_clientSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        // TODO: correctly close socket
        m_clientSocket = 0;
        return false;
    }

    return true;
}

} // namespace Network
} // namespace Kitsune
