/**
 *  @file    income_trigger.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "income_trigger.h"
#include <tcp/tcp_socket.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
IncomeTrigger::IncomeTrigger()
    : ConnectionTrigger ()
{
}

/**
 * destructor
 */
IncomeTrigger::~IncomeTrigger()
{
}

/**
 * runTask
 */
void
IncomeTrigger::handleConnection(AbstractSocket* socket)
{
    socket->start();
}

} // namespace Network
} // namespace Kitsune
