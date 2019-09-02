#include "abstract_server.h"

namespace Kitsune
{
namespace Network
{

/**
 * @brief AbstractServer::AbstractServer
 */
AbstractServer::AbstractServer()
{

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
 * @return tcp-client-pointer
 */
AbstractClient*
AbstractServer::getSocket(const uint32_t pos)
{
    AbstractClient* result = nullptr;
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
AbstractServer::addAdditionalTrigger(NetworkTrigger *trigger)
{
    m_trigger.push_back(trigger);
}

/**
 * delete all trigger-objects from the server
 */
void
AbstractServer::clearTrigger()
{
    m_trigger.clear();
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
