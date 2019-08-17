/**
 *  @file    tcp_server.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <threading/thread.h>

namespace Kitsune
{
namespace Network
{
class NetworkTrigger;
class TcpClient;

class TcpServer : public Kitsune::Common::Thread
{
public:
    TcpServer(NetworkTrigger* trigger = nullptr);
    ~TcpServer();

    bool initSocket(const uint16_t port);
    TcpClient* waitForIncomingConnection();
    bool closeServer();

    bool sendMessage(const std::string &message, int clientSocket);

    uint64_t getNumberOfSockets();
    TcpClient* getSocket(const uint32_t pos);

    // trigger-control
    void addAdditionalTrigger(NetworkTrigger* trigger);
    void clearTrigger();

protected:
    void run();

private:
    int m_serverSocket = 0;
    std::vector<NetworkTrigger*> m_trigger;
    std::vector<TcpClient*> m_sockets;
    struct sockaddr_in m_server;
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_SERVER_H
