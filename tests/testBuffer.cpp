#include "testBuffer.h"
#include <tcp/tcpClient.h>

#include <buffering/commonDataBuffer.h>
#include <buffering/commonDataBufferMethods.h>

namespace Kitsune
{
namespace Network
{


TestBuffer::TestBuffer() : NetworkTrigger ()
{
    m_buffer = new CommonDataBuffer(1000);
}

TestBuffer::~TestBuffer()
{
    delete m_buffer;
}

uint32_t
TestBuffer::runTask(const MessageRingBuffer &recvBuffer,
                    TcpClient *client)
{
    uint8_t buffer[RECV_BUFFER_SIZE];
    const uint8_t* dataPointer = getDataPointer(recvBuffer, buffer, recvBuffer.readWriteDiff);
    addDataToBuffer(m_buffer, dataPointer, recvBuffer.readWriteDiff);
    return recvBuffer.readWriteDiff;
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
    resetBuffer(m_buffer, 1000);
}

}
}
