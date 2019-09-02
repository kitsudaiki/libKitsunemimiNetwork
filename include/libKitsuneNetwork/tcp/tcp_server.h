/**
 *  @file    tcp_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
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
#include <vector>

#include <abstract_server.h>

namespace Kitsune
{
namespace Network
{
class TcpClient;

class TcpServer : public AbstractServer
{
public:
    TcpServer(NetworkTrigger* trigger = nullptr);
    ~TcpServer();

    bool initSocket(const uint16_t port);
    AbstractClient* waitForIncomingConnection();
    bool closeServer();

private:
    struct sockaddr_in m_server;
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_SERVER_H
