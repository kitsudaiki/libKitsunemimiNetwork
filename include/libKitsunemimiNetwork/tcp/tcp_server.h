/**
 *  @file    tcp_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <libKitsunemimiCommon/logger.h>

#include <libKitsunemimiNetwork/tcp/tcp_socket.h>

namespace Kitsunemimi
{
namespace Network
{
template<class>
class NetSocket;

class TcpServer
{
public:
    TcpServer(const uint16_t port);
    TcpServer();
    ~TcpServer();

    bool initServer(ErrorContainer &error);

    int serverFd = 0;
    uint32_t type = 0;;

    uint16_t m_port = 0;
    struct sockaddr_in m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TCP_SERVER_H
