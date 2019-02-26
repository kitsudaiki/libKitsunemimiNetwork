#ifndef TESTBUFFER_H
#define TESTBUFFER_H

#include <networkTrigger.h>

namespace Kitsune
{
class CommonDataBuffer;

namespace Network
{

class TestBuffer : public NetworkTrigger
{
public:
    TestBuffer();
    ~TestBuffer();
    void runTask(uint8_t* buffer, const long bufferSize);

    uint64_t getNumberOfWrittenBytes();
    CommonDataBuffer *getBuffer();

    void clearBuffer();

private:
    CommonDataBuffer* m_buffer = nullptr;
};

}
}

#endif // TESTBUFFER_H
