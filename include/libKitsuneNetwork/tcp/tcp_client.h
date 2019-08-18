/**
 *  @file    tcp_client.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <cinttypes>
#include <unistd.h>
#include <string>
#include <vector>

#include <threading/thread.h>
#include <buffering/data_buffer.h>

#include <message_ring_buffer.h>

namespace Kitsune
{
namespace Network
{
class NetworkTrigger;
class CleanupThread;

class TcpClient : public Kitsune::Common::Thread
{
public:
    TcpClient(const std::string address,
              const uint16_t port);
    TcpClient(const int clientFd,
              struct sockaddr_in client);
    ~TcpClient();

    static Kitsune::Network::CleanupThread* m_cleanup;

    // trigger-control
    bool addNetworkTrigger(NetworkTrigger* trigger);
    bool removeNetworkTrigger(const uint32_t index);
    void clearNetworkTrigger();

    // send
    bool sendMessage(const std::string &message);
    bool sendMessage(const uint8_t *message,
                     const uint64_t numberOfBytes);

    bool closeSocket();

protected:
    void run();

private:
    bool m_clientSide = false;
    std::string m_address = "";
    uint16_t m_port = 0;
    int m_clientSocket = 0;
    sockaddr_in m_client;

    MessageRingBuffer m_recvBuffer;
    std::vector<NetworkTrigger*> m_trigger;

    bool initClientSide();
    bool waitForMessage();
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_CLIENT_H
