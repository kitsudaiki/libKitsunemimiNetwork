#ifndef TESTBUFFER_H
#define TESTBUFFER_H

#include <networkTrigger.h>

namespace Kitsune
{
class CommonDataBuffer;

namespace Network
{
class TcpClient;

class TestBuffer : public NetworkTrigger
{
public:
    TestBuffer();
    ~TestBuffer();
    uint32_t runTask(uint8_t* buffer,
                     const uint32_t bufferStart,
                     const uint32_t bufferSize,
                     const uint32_t totalBufferSize,
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
