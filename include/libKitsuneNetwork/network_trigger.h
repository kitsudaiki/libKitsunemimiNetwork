/**
 *  @file    network_trigger.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef NETWORK_TRIGGER_H
#define NETWORK_TRIGGER_H

#include <cstdio>
#include <cinttypes>

namespace Kitsune
{
namespace Network
{
class AbstractSocket;
struct MessageRingBuffer;

class NetworkTrigger
{
public:
    NetworkTrigger();
    virtual ~NetworkTrigger();

    virtual uint64_t runTask(MessageRingBuffer& recvBuffer,
                             AbstractSocket* socket) = 0;
};

} // namespace Network
} // namespace Kitsune

#endif // NETWORK_TRIGGER_H
