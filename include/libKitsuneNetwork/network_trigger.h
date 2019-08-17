/**
 *  @file    network_trigger.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef NETWORK_TRIGGER_H
#define NETWORK_TRIGGER_H

#include <cstdio>
#include <cinttypes>

namespace Kitsune
{
namespace Network
{
class TcpClient;
struct MessageRingBuffer;

class NetworkTrigger
{
public:
    NetworkTrigger();
    virtual ~NetworkTrigger();

    virtual uint64_t runTask(const MessageRingBuffer& recvBuffer,
                             TcpClient* client) = 0;
};

} // namespace Network
} // namespace Kitsune

#endif // NETWORK_TRIGGER_H
