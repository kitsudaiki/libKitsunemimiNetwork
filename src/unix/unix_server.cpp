/**
 *  @file    unix_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <unix/unix_socket.h>
#include <unix/unix_server.h>
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
UnixServer::UnixServer(NetworkTrigger* trigger)
{
    m_trigger.push_back(trigger);
}

/**
 * destructor
 */
UnixServer::~UnixServer()
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
UnixServer::initServer(const std::string socketFile)
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
UnixServer::waitForIncomingConnection()
{
    uint32_t length = sizeof(struct sockaddr_un);

    //make new connection
    int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);
    if(fd < 0)
    {
        LOG_error("Failed accept incoming connection on unix-server with address: " + m_socketFile);
        return nullptr;
    }

    LOG_info("Successfully accepted incoming connection on unix-socket server with "
             "address : " + m_socketFile);

    // create new socket-object from file-descriptor
    UnixSocket* unixSocket = new UnixSocket(fd);
    for(uint32_t i = 0; i < m_trigger.size(); i++) 
    {
        unixSocket->addNetworkTrigger(m_trigger.at(i));
    }

    // start new socket-thread
    unixSocket->start();

    // append new socket to the list
    mutexLock();
    m_sockets.push_back(unixSocket);
    mutexUnlock();

    return unixSocket;
}

} // namespace Network
} // namespace Kitsune

