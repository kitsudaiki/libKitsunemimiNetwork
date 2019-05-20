#include "testBuffer.h"
#include <buffering/commonDataBuffer.h>
#include <buffering/commonDataBufferMethods.h>

namespace Kitsune
{
namespace Network
{


TestBuffer::TestBuffer() : NetworkTrigger ()
{
    m_buffer = new CommonDataBuffer();
}

TestBuffer::~TestBuffer()
{
    delete m_buffer;
}

uint32_t
TestBuffer::runTask(uint8_t *buffer,
                    const uint32_t bufferStart,
                    const uint32_t bufferSize,
                    const uint32_t totalBufferSize,
                    TcpClient *client)
{
    if(bufferStart + bufferSize  > totalBufferSize)
    {
        const uint32_t firstPart = ((bufferStart + bufferSize) % totalBufferSize) - bufferStart;
        addDataToBuffer(m_buffer, &buffer[bufferStart], firstPart);
        addDataToBuffer(m_buffer, &buffer[0], bufferSize - firstPart);
    }
    else {
        addDataToBuffer(m_buffer, &buffer[bufferStart], bufferSize);
    }
    return bufferSize;
}

CommonDataBuffer*
TestBuffer::getBuffer()
{
    return m_buffer;
}

uint64_t
TestBuffer::getNumberOfWrittenBytes()
{
    return m_buffer->bufferPosition;
}

void
TestBuffer::clearBuffer()
{
    resetBuffer(m_buffer);
}

}
}
