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
    m_buffer = new CommonDataBuffer();
}

TestBuffer::~TestBuffer()
{
    delete m_buffer;
}

uint32_t
TestBuffer::runTask(const MessageRingBuffer &recvBuffer,
                    TcpClient *client)
{
    if(recvBuffer.readPosition + recvBuffer.readWriteDiff  > recvBuffer.totalBufferSize)
    {
        const uint32_t firstPart = ((recvBuffer.readPosition + recvBuffer.readWriteDiff)
                                    % recvBuffer.totalBufferSize) - recvBuffer.readPosition;
        addDataToBuffer(m_buffer, &recvBuffer.data[recvBuffer.readPosition], firstPart);
        addDataToBuffer(m_buffer, &recvBuffer.data[0], recvBuffer.readWriteDiff - firstPart);
    }
    else {
        addDataToBuffer(m_buffer, &recvBuffer.data[recvBuffer.readPosition], recvBuffer.readWriteDiff);
    }
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
    resetBuffer(m_buffer);
}

}
}
