/**
 *  @file    tcp_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <netdb.h>
#include <netinet/tcp.h>
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
#include <atomic>

#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Network
{
class UnixDomainServer;
class TcpServer;
class TlsTcpServer;
class TlsTcpSocket;

template <class>
class NetSocket;

template <class>
class NetServer;

class TcpSocket
{
public:
    TcpSocket(const std::string &address,
              const uint16_t port);
    ~TcpSocket();

private:
    friend NetSocket<TcpSocket>;
    friend NetServer<UnixDomainServer>;
    friend NetServer<TcpServer>;
    friend NetServer<TlsTcpServer>;
    friend TlsTcpSocket;

    TcpSocket();

    sockaddr_in socketAddr;
    bool isConnected = false;
    bool isClientSide = false;
    int socketFd = 0;
    uint32_t type = 0;

    TcpSocket(const int socketFd);

    bool initClientSide(ErrorContainer &error);
    bool initSocket(ErrorContainer &error);
    int getSocketFd() const;
    long recvData(int socket,
                  void* bufferPosition,
                  const size_t bufferSize,
                  int flags);
    ssize_t sendData(int socket,
                     const void* bufferPosition,
                     const size_t bufferSize,
                     int flags);

    const std::string &getAddress() const;

    std::string m_address = "";
    uint16_t m_port = 0;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TCP_SOCKET_H
