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
class MessageTrigger;
class CleanupThread;

class AbstractSocket : public Kitsune::Common::Thread
{
public:
    enum socketTypes {
        UNDEFINED_TYPE = 0,
        UNIX_SOCKET = 1,
        TCP_SOCKET = 2,
        TLS_TCP_SOCKET = 3
    };

    AbstractSocket();
    ~AbstractSocket();

    virtual bool initClientSide() = 0;
    socketTypes getType();

    // trigger-control
    void setMessageTrigger(MessageTrigger* trigger);

    bool sendMessage(const std::string &message);
    bool sendMessage(const void *message,
                     const uint64_t numberOfBytes);

    bool closeSocket();

    static Kitsune::Network::CleanupThread* m_cleanup;

protected:
    bool m_clientSide = false;
    int m_socket = 0;
    socketTypes m_type = UNDEFINED_TYPE;

    MessageRingBuffer m_recvBuffer;
    MessageTrigger* m_trigger;

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
