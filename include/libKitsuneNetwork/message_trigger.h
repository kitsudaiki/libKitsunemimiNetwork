/**
 *  @file    message_trigger.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef MESSAGE_TRIGGER_H
#define MESSAGE_TRIGGER_H

#include <cstdio>
#include <cinttypes>

namespace Kitsune
{
namespace Network
{
class AbstractSocket;
struct MessageRingBuffer;

class MessageTrigger
{
public:
    MessageTrigger();
    virtual ~MessageTrigger();

    virtual uint64_t runTask(MessageRingBuffer& recvBuffer,
                             AbstractSocket* socket) = 0;
};

} // namespace Network
} // namespace Kitsune

#endif // MESSAGE_TRIGGER_H
