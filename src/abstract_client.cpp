/**
 *  @file    abstract_client.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "abstract_client.h"

#include <cleanup_thread.h>
#include <network_trigger.h>

namespace Kitsune
{
namespace Network
{

Kitsune::Network::CleanupThread* AbstractClient::m_cleanup = nullptr;

/**
 * @brief AbstractClient::AbstractClient
 */
AbstractClient::AbstractClient()
{
    if(m_cleanup == nullptr)
    {
        m_cleanup = new Kitsune::Network::CleanupThread();
        m_cleanup->start();
    }
}

/**
 * @brief AbstractClient::~AbstractClient
 */
AbstractClient::~AbstractClient()
{
    clearNetworkTrigger();
}

/**
 * add new trigger-object for incoming messages
 *
 * @param trigger new trigger-object
 *
 * @return false, if object was nullptr, else true
 */
bool
AbstractClient::addNetworkTrigger(NetworkTrigger *trigger)
{
    if(trigger == nullptr) {
        return false;
    }

    m_trigger.push_back(trigger);

    return true;
}

/**
 * remove a specific trigger from the client
 *
 * @param index index of the trigger in the list
 *
 * @return false, if index too high, else tru
 */
bool
AbstractClient::removeNetworkTrigger(const uint32_t index)
{
    if(m_trigger.size() <= index) {
        return false;
    }

    m_trigger.erase(m_trigger.begin() + index);

    return true;
}

/**
 * delete all trigger-objects from the client
 */
void
AbstractClient::clearNetworkTrigger()
{
    m_trigger.clear();
}

/**
 * run-method for the thread-class
 */
void
AbstractClient::run()
{
    while(!m_abort)
    {
        waitForMessage();
    }
}

} // namespace Network
} // namespace Kitsune
