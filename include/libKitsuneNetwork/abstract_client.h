#ifndef ABSTRACT_CLIENT_H
#define ABSTRACT_CLIENT_H
/**
 *  @file    abstract_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


    virtual bool sendMessage(const std::string &message) = 0;
    virtual bool sendMessage(const uint8_t *message,
                             const uint64_t numberOfBytes) = 0;

    virtual bool closeSocket() = 0;

protected:
    bool m_clientSide = false;
    MessageRingBuffer m_recvBuffer;
    std::vector<NetworkTrigger*> m_trigger;

    void run();

private:
    virtual bool initClientSide() = 0;
    virtual bool waitForMessage() = 0;
};

} // namespace Network
} // namespace Kitsune

#endif // ABSTRACT_CLIENT_H
