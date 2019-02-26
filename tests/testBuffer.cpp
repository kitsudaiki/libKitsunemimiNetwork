#include "testBuffer.h"
#include <commonDataBuffer.h>

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
TestBuffer::runTask(uint8_t *buffer, const long bufferSize)
{
    m_buffer->addData(buffer, static_cast<uint32_t>(bufferSize));
}

CommonDataBuffer*
TestBuffer::getBuffer()
{
    return m_buffer;
}

uint64_t
TestBuffer::getNumberOfWrittenBytes()
{
    return m_buffer->getNumberOfWrittenBytes();
}

void
TestBuffer::clearBuffer()
{
    m_buffer->resetBuffer();
}

}
}
