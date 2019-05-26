#ifndef MESSAGERINGBUFFER_H
#define MESSAGERINGBUFFER_H

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
    uint32_t totalBufferSize = RECV_BUFFER_SIZE;
    uint32_t readPosition = 0;
    uint32_t readWriteDiff = 0;
};


inline const uint8_t*
getDataPointer(const MessageRingBuffer &recvBuffer,
               uint8_t* backupPointer,
               const uint32_t size)
{
    if(recvBuffer.readPosition + size  > recvBuffer.totalBufferSize)
    {
        // only copy data, when absolut necessary
        const uint32_t firstPart = size - ((recvBuffer.readPosition + size) % recvBuffer.totalBufferSize);
        mempcpy(backupPointer, &recvBuffer.data[recvBuffer.readPosition], firstPart);
        mempcpy(&backupPointer[firstPart], &recvBuffer.data[0], size - firstPart);
        return backupPointer;
    }

    return &recvBuffer.data[recvBuffer.readPosition];
}

}
}

#endif // MESSAGERINGBUFFER_H
