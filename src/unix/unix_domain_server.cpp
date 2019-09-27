/**
 *  @file    unix_domain_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <unix/unix_domain_socket.h>
#include <unix/unix_domain_server.h>
#include <message_trigger.h>
#include <connection_trigger.h>
#include <logger/logger.h>

using namespace Kitsune::Persistence;

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
UnixDomainServer::UnixDomainServer(MessageTrigger* messageTrigger,
                                   ConnectionTrigger* connectionTrigger)
    : AbstractServer(messageTrigger, connectionTrigger)
{
    m_type = UNIX_SERVER;
}

/**
 * destructor
 */
UnixDomainServer::~UnixDomainServer()
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
UnixDomainServer::initServer(const std::string socketFile)
{
    m_socketFile = socketFile;

    // create socket
    m_serverSocket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(m_serverSocket < 0)
    {
        LOG_error("Failed to create a unix-socket");
        return false;
    }

    unlink(m_socketFile.c_str());
    m_server.sun_family = AF_LOCAL;
    strncpy(m_server.sun_path, m_socketFile.c_str(), m_socketFile.size());

    // bind to port
    if(bind(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), sizeof(m_server)) < 0)
    {
        LOG_error("Failed to bind unix-socket to addresse: " + m_socketFile);
        return false;
    }

    // start listening for incoming connections
    if(listen(m_serverSocket, 5) == -1)
    {
        LOG_error("Failed listen on unix-socket on addresse: " + m_socketFile);
        return false;
    }

    LOG_info("Successfully initialized unix-socket server on targe: " + m_socketFile);

    return true;
}

/**
 * wait for new incoming unix-socket-connections
 *
 * @return new unix-socket-socket-pointer for the new incoming connection
 */
AbstractSocket*
UnixDomainServer::waitForIncomingConnection()
{
    uint32_t length = sizeof(struct sockaddr_un);

    //make new connection
    int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);

    if(m_abort) {
        return nullptr;
    }

    if(fd < 0)
    {
        LOG_error("Failed accept incoming connection on unix-server with address: " + m_socketFile);
        return nullptr;
    }

    LOG_info("Successfully accepted incoming connection on unix-socket server with "
             "address : " + m_socketFile);

    // create new socket-object from file-descriptor
    UnixDomainSocket* unixSocket = new UnixDomainSocket(fd);
    unixSocket->addNetworkTrigger(m_messageTrigger);
    m_connectionTrigger->handleConnection(unixSocket);

    // append new socket to the list
    mutexLock();
    m_sockets.push_back(unixSocket);
    mutexUnlock();

    return unixSocket;
}

} // namespace Network
} // namespace Kitsune

