/**
 *  @file    dummyBuffer.hpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef DUMMYBUFFER_HPP
#define DUMMYBUFFER_HPP

#include <networkTrigger.hpp>

namespace Kitsune
{
class CommonDataBuffer;

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
    CommonDataBuffer *getBuffer();

    void clearBuffer();

private:
    CommonDataBuffer* m_buffer = nullptr;
};

}
}

#endif // DUMMYBUFFER_HPP
