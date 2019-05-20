/**
 *  @file    networkTrigger.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef NETWORKTRIGGER_H
#define NETWORKTRIGGER_H

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
    virtual uint32_t runTask(const MessageRingBuffer& recvBuffer,
                             TcpClient* client) = 0;
};

}
}

#endif // NETWORKTRIGGER_H
