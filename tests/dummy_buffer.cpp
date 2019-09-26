/**
 *  @file    dummy_buffer.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "dummy_buffer.h"
#include <tcp/tcp_socket.h>

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
    : MessageTrigger ()
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
DummyBuffer::runTask(MessageRingBuffer &recvBuffer,
                     AbstractSocket*)
{
    const uint8_t* dataPointer = getDataPointer(recvBuffer, recvBuffer.readWriteDiff);

    if(dataPointer == nullptr) {
        return 0;
    }

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

} // namespace Network
} // namespace Kitsune
