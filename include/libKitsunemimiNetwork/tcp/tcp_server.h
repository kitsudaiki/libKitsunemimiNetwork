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
class TlsTcpServer;

template<class>
class NetSocket;

template <class>
class NetServer;

class TcpServer
{
public:
    TcpServer(const uint16_t port);
    ~TcpServer();

    bool initServer(ErrorContainer &error);

private:
    friend NetServer<TcpServer>;
    friend TlsTcpServer;

    TcpServer();

    int getServerFd() const;
    uint16_t getPort() const;

    int serverFd = 0;
    uint32_t type = 0;;
    std::string caFile = "";
    std::string certFile = "";
    std::string keyFile = "";
    struct sockaddr_in socketAddr;

    uint16_t m_port = 0;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TCP_SERVER_H
