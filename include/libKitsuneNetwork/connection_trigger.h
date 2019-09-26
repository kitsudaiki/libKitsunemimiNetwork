/**
 *  @file    connection_trigger.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef CONNECTION_TRIGGER_H
#define CONNECTION_TRIGGER_H

#include <cstdio>
#include <cinttypes>

namespace Kitsune
{
namespace Network
{
class AbstractSocket;

class ConnectionTrigger
{
public:
    ConnectionTrigger();
    virtual ~ConnectionTrigger();

    virtual void handleConnection(AbstractSocket* connection) = 0;
};

} // namespace Network
} // namespace Kitsune

#endif // CONNECTION_TRIGGER_H
