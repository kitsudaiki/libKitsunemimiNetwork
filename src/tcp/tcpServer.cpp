/**
 *  @file    tcpServer.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include <tcp/tcpServer.hpp>
#include <tcp/tcpClient.hpp>
#include <networkTrigger.hpp>
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
TcpClient*
TcpServer::waitForIncomingConnection()
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

/**
 * close the tcp-server togester with all tcp-client, which are connected to the server
 *
 * @return false, if already closed, else true
 */
bool
TcpServer::closeServer()
{
    // precheck
    if(m_abort == true) {
        return false;
    }

    m_abort = true;

    // close server-socket
    if(m_serverSocket >= 0)
    {   
        // close server itself
        shutdown(m_serverSocket, SHUT_RDWR);
        close(m_serverSocket);
        m_serverSocket = 0;
    }

    // stop thread
    stop();

    // close all connected sockets
    mutexLock();
    for(uint32_t i = 0; i < m_sockets.size(); i++)
    {
        m_sockets[i]->closeSocket();
    }
    m_sockets.clear();
    mutexUnlock();

    return true;
}

/**
 * get the number of sockets which are registered at the server
 */
uint64_t
TcpServer::getNumberOfSockets()
{
    uint64_t result = 0;
    mutexLock();
    result = m_sockets.size();
    mutexUnlock();
    return result;
}

/**
 * get a specific tcp-socket from the server
 *
 * @param pos position in the list
 *
 * @return tcp-client-pointer
 */
TcpClient*
TcpServer::getSocket(const uint32_t pos)
{
    TcpClient* result = nullptr;
    mutexLock();
    if(pos < m_sockets.size()) {
        result = m_sockets.at(pos);
    }
    mutexUnlock();
    return result;
}

/**
 * add a new trigger to the server
 *
 * @param trigger new trigger-object
 */
void
TcpServer::addAdditionalTrigger(NetworkTrigger *trigger)
{
    m_trigger.push_back(trigger);
}

/**
 * delete all trigger-objects from the server
 */
void
TcpServer::clearTrigger()
{
    m_trigger.clear();
}

/**
 * run-method for the thread-class
 */
void
TcpServer::run()
{
    while(!m_abort)
    {
        waitForIncomingConnection();
    }
}

} // namespace Network
} // namespace Kitsune
