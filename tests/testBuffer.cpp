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

void
TestBuffer::runTask(uint8_t *buffer, const long bufferSize, TcpClient *client)
{
    addDataToBuffer(m_buffer, buffer, static_cast<uint32_t>(bufferSize));
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
