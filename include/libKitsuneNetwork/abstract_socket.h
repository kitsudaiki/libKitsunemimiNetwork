/**
 *  @file    abstract_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef ABSTRACT_SOCKET_H
#define ABSTRACT_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cinttypes>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <errno.h>

#include <buffering/data_buffer.h>
#include <message_ring_buffer.h>
#include <threading/thread.h>

namespace Kitsune
{
namespace Network
{
class NetworkTrigger;
class CleanupThread;

class AbstractSocket : public Kitsune::Common::Thread
{
public:
    AbstractSocket();
    ~AbstractSocket();

    static Kitsune::Network::CleanupThread* m_cleanup;

    // trigger-control
    bool addNetworkTrigger(NetworkTrigger* trigger);
    bool removeNetworkTrigger(const uint32_t index);
    void clearNetworkTrigger();

    bool sendMessage(const std::string &message);
    bool sendMessage(const void *message,
                     const uint64_t numberOfBytes);

    bool closeSocket();

protected:
    bool m_clientSide = false;
    int m_socket = 0;

    MessageRingBuffer m_recvBuffer;
    std::vector<NetworkTrigger*> m_trigger;

    void run();
    bool waitForMessage();

    virtual bool initSocket() = 0;
    virtual long recvData(int socket,
                          void* bufferPosition,
                          const size_t bufferSize,
                          int flags) = 0;
    virtual ssize_t sendData(int socket,
                             const void* bufferPosition,
                             const size_t bufferSize,
                             int flags) = 0;
};

} // namespace Network
} // namespace Kitsune

#endif // ABSTRACT_SOCKET_H
