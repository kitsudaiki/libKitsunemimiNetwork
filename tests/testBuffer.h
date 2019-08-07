#ifndef TESTBUFFER_H
#define TESTBUFFER_H

#include <networkTrigger.hpp>

namespace Kitsune
{
class CommonDataBuffer;

namespace Network
{
class TcpClient;
struct MessageRingBuffer;

class TestBuffer : public NetworkTrigger
{
public:
    TestBuffer();
    ~TestBuffer();
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

#endif // TESTBUFFER_H
