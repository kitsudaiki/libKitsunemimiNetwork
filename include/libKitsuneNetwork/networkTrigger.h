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

class NetworkTrigger
{
public:
    NetworkTrigger();
    virtual ~NetworkTrigger();
    virtual uint32_t runTask(uint8_t* buffer, const uint32_t bufferStart,
                             const uint32_t bufferSize, const uint32_t totalBufferSize,
                             TcpClient* client) = 0;
};

}
}

#endif // NETWORKTRIGGER_H
