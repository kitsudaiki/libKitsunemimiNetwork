/**
 *  @file    dummyBuffer.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "dummyBuffer.hpp"
#include <tcp/tcpClient.hpp>

#include <buffering/commonDataBuffer.hpp>
#include <buffering/commonDataBufferMethods.hpp>

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
    m_buffer = new CommonDataBuffer(1000);
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
CommonDataBuffer*
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
