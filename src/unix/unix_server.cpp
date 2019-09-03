/**
 *  @file    unix_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <unix/unix_client.h>
#include <unix/unix_server.h>
#include <network_trigger.h>
#include <iostream>

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
UnixServer::initSocket(const std::string socketFile)
{
    // create socket
    m_serverSocket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(m_serverSocket < 0) {
        return false;
    }

    unlink(socketFile.c_str());
    m_server.sun_family = AF_LOCAL;
    strncpy(m_server.sun_path, socketFile.c_str(), socketFile.size());

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
 * wait for new incoming unix-socket-connections
 *
 * @return new unix-client-socket-pointer for the new incoming connection
 */
AbstractClient*
UnixServer::waitForIncomingConnection()
{
    struct sockaddr_un client;
    uint32_t length = sizeof(client);

    //make new connection
    int fd = accept(m_serverSocket, (struct sockaddr*)&client, &length);
    if(fd < 0) {
        return nullptr;
    }

    // create new client-object from file-descriptor
    UnixClient* unixClient = new UnixClient(fd, client);
    for(uint32_t i = 0; i < m_trigger.size(); i++) 
    {
        unixClient->addNetworkTrigger(m_trigger.at(i));
    }

    // start new client-thread
    unixClient->start();

    // append new socket to the list
    mutexLock();
    m_sockets.push_back(unixClient);
    mutexUnlock();

    return unixClient;
}

} // namespace Network
} // namespace Kitsune

