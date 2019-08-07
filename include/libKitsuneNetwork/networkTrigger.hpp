/**
 *  @file    networkTrigger.hpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef NETWORKTRIGGER_HPP
#define NETWORKTRIGGER_HPP

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

#endif // NETWORKTRIGGER_HPP
