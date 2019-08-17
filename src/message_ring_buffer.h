/**
 *  @file    message_ring_buffer.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef MESSAGE_RING_BUFFER_H
#define MESSAGE_RING_BUFFER_H

#include <cinttypes>
#include <string.h>

namespace Kitsune
{
namespace Network
{

#define RECV_BUFFER_SIZE 8192


struct MessageRingBuffer
{
    uint8_t data[RECV_BUFFER_SIZE];
    uint64_t totalBufferSize = RECV_BUFFER_SIZE;
    uint64_t readPosition = 0;
    uint64_t readWriteDiff = 0;
};

/**
 * get a pointer to the complete monolitic block of the ring-buffer
 *
 * @param recvBuffer ring-buffer-object with the complete data
 * @param overflowBuffer buffer for data-copy, if the requested block is splitted inside the
 *                       ring-buffer. Musst be pre-allocated and must have at least the
 *                       size of the requested bloc
 * @param size size of the requested block
 * @param offset offset position from the current read-position
 *
 * @return pointer to the beginning of the requested datablock
 */
inline const uint8_t*
getDataPointer(const MessageRingBuffer &recvBuffer,
               uint8_t* overflowBuffer,
               const uint64_t size,
               const uint64_t offset = 0)
{
    const uint64_t startPosition = (recvBuffer.readPosition + offset) % recvBuffer.totalBufferSize;

    // check if requested datablock is splitet
    if(startPosition + size > recvBuffer.totalBufferSize)
    {
        // copy the two parts of the requested block into the overflow-buffer
        const uint64_t firstPart = size - ((startPosition + size) % recvBuffer.totalBufferSize);
        mempcpy(overflowBuffer, &recvBuffer.data[startPosition], firstPart);
        mempcpy(&overflowBuffer[firstPart], &recvBuffer.data[0], size - firstPart);
        return overflowBuffer;
    }

    return &recvBuffer.data[startPosition];
}

} // namespace Network
} // namespace Kitsune

#endif // MESSAGE_RING_BUFFER_H
