/**
 *  @file    dummy_buffer.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "dummy_buffer.h"
#include <tcp/tcp_client.h>

#include <buffering/data_buffer.h>
#include <buffering/data_buffer_methods.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
DummyBuffer::DummyBuffer()
    : NetworkTrigger ()
{
    m_buffer = new Common::DataBuffer(1000);
}

/**
 * destructor
 */
DummyBuffer::~DummyBuffer()
{
    delete m_buffer;
}

/**
 * runTask
 */
uint64_t
DummyBuffer::runTask(const MessageRingBuffer &recvBuffer,
                     TcpClient *client)
{
    uint8_t buffer[RECV_BUFFER_SIZE];
    const uint8_t* dataPointer = getDataPointer(recvBuffer, buffer, recvBuffer.readWriteDiff);
    addDataToBuffer(m_buffer, dataPointer, recvBuffer.readWriteDiff);
    return recvBuffer.readWriteDiff;
}

/**
 * getBuffer
 */
Common::DataBuffer*
DummyBuffer::getBuffer()
{
    return m_buffer;
}

/**
 * getNumberOfWrittenBytes
 */
uint64_t
DummyBuffer::getNumberOfWrittenBytes()
{
    return m_buffer->bufferPosition;
}

/**
 * clearBuffer
 */
void
DummyBuffer::clearBuffer()
{
    resetBuffer(m_buffer, 1000);
}

}
}
