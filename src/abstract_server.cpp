/**
 *  @file    abstract_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/abstract_server.h>
#include <libKitsunemimiNetwork/abstract_socket.h>
#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief AbstractServer::AbstractServer
 */
AbstractServer::AbstractServer(void* target,
                               void (*processConnection)(void*, AbstractSocket*),
                               const std::string &threadName)
    : Kitsunemimi::Thread(threadName)
{
    m_target = target;
    m_processConnection = processConnection;
}

/**
 * @brief AbstractServer::~AbstractServer
 */
AbstractServer::~AbstractServer()
{
}

/**
 * @brief get type of the server
 *
 * @return type of server
 */
AbstractServer::serverTypes
AbstractServer::getType()
{
    return m_type;
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

    LOG_INFO("Successfully closed server");

    return true;
}

/**
 * run-method for the thread-class
 */
void
AbstractServer::run()
{
    ErrorContainer error;
    while(m_abort == false)
    {
        if(waitForIncomingConnection(error) == false)
        {
            LOG_ERROR(error);
            error._alreadyPrinted = false;
        }
    }
}

} // namespace Network
} // namespace Kitsunemimi
