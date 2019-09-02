/**
 *  @file    dummy_buffer.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef DUMMYBUFFER_H
#define DUMMYBUFFER_H

#include <network_trigger.h>

namespace Kitsune
{
namespace Common
{
class DataBuffer;
}

namespace Network
{
class TcpClient;
struct MessageRingBuffer;

class DummyBuffer : public NetworkTrigger
{
public:
    DummyBuffer();
    ~DummyBuffer();
    uint64_t runTask(const MessageRingBuffer& recvBuffer,
                     TcpClient* client);

    uint64_t getNumberOfWrittenBytes();
    Common::DataBuffer* getBuffer();

    void clearBuffer();

private:
    Common::DataBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsune

#endif // DUMMYBUFFER_H
