/**
 *  @file    abstract_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "abstract_server.h"
#include <abstract_socket.h>
#include <connection_trigger.h>
#include <logger/logger.h>

using namespace Kitsune::Persistence;

namespace Kitsune
{
namespace Network
{

/**
 * @brief AbstractServer::AbstractServer
 */
AbstractServer::AbstractServer(MessageTrigger* messageTrigger,
                               ConnectionTrigger* connectionTrigger)
{
    m_messageTrigger = messageTrigger;
    m_connectionTrigger = connectionTrigger;
}

/**
 * @brief AbstractServer::~AbstractServer
 */
AbstractServer::~AbstractServer()
{
}

/**
 * get the number of sockets which are registered at the server
 */
uint64_t
AbstractServer::getNumberOfSockets()
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
 * @return tcp-socket-pointer
 */
AbstractSocket*
AbstractServer::getSocket(const uint32_t pos)
{
    AbstractSocket* result = nullptr;
    mutexLock();
    if(pos < m_sockets.size()) {
        result = m_sockets.at(pos);
    }
    mutexUnlock();
    return result;
}

/**
 * close the tcp-server togester with all tcp-socket, which are connected to the server
 *
 * @return false, if already closed, else true
 */
bool
AbstractServer::closeServer()
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

    LOG_info("Successfully closed server");

    return true;
}

/**
 * run-method for the thread-class
 */
void
AbstractServer::run()
{
    while(!m_abort)
    {
        waitForIncomingConnection();
    }
}

} // namespace Network
} // namespace Kitsune
