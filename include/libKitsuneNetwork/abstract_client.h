#ifndef ABSTRACT_CLIENT_H
#define ABSTRACT_CLIENT_H
/**
 *  @file    abstract_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

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

class AbstractClient : public Kitsune::Common::Thread
{
public:
    AbstractClient();
    ~AbstractClient();

    static Kitsune::Network::CleanupThread* m_cleanup;

    // trigger-control
    bool addNetworkTrigger(NetworkTrigger* trigger);
    bool removeNetworkTrigger(const uint32_t index);
    void clearNetworkTrigger();

    bool sendMessage(const std::string &message);
    bool sendMessage(const uint8_t *message,
                     const uint64_t numberOfBytes);

    bool closeSocket();

protected:
    bool m_clientSide = false;
    int m_clientSocket = 0;

    MessageRingBuffer m_recvBuffer;
    std::vector<NetworkTrigger*> m_trigger;

    void run();

private:
    virtual bool initClientSide() = 0;
    bool waitForMessage();
};

} // namespace Network
} // namespace Kitsune

#endif // ABSTRACT_CLIENT_H
