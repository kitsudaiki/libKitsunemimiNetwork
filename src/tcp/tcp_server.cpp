/**
 *  @file    tcp_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <tcp/tcp_server.h>
#include <tcp/tcp_client.h>
#include <network_trigger.h>
#include <iostream>

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
TcpServer::TcpServer(NetworkTrigger* trigger)
{
    m_trigger.push_back(trigger);
}

/**
 * destructor
 */
TcpServer::~TcpServer()
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
TcpServer::initSocket(const uint16_t port)
{
    // create socket
    m_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_serverSocket < 0) {
        return false;
    }

    // make the port reusable
    int enable = 1;
    if(setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        return false;
    }

    // set server-settings
    memset(&m_server, 0, sizeof (m_server));
    m_server.sin_family = AF_INET;
    m_server.sin_addr.s_addr = htonl(INADDR_ANY);
    m_server.sin_port = htons(port);

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
 * wait for new incoming tcp-connections
 *
 * @return new tcp-client-socket-pointer for the new incoming connection
 */
AbstractClient* TcpServer::waitForIncomingConnection()
{
    struct sockaddr_in client;
    uint32_t length = sizeof(client);

    //make new connection
    int fd = accept(m_serverSocket, (struct sockaddr*)&client, &length);
    if(fd < 0) {
        return nullptr;
    }

    // create new client-object from file-descriptor
    TcpClient* tcpClient = new TcpClient(fd, client);
    for(uint32_t i = 0; i < m_trigger.size(); i++) 
    {
        tcpClient->addNetworkTrigger(m_trigger.at(i));
    }

    // start new client-thread
    tcpClient->start();

    // append new socket to the list
    mutexLock();
    m_sockets.push_back(tcpClient);
    mutexUnlock();

    return tcpClient;
}

} // namespace Network
} // namespace Kitsune
