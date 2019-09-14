/**
 *  @file    tcp_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <abstract_socket.h>

namespace Kitsune
{
namespace Network
{
class TcpServer;

class TcpSocket : public AbstractSocket
{
    friend class TcpServer;

public:
    TcpSocket(const std::string address,
              const uint16_t port);

    bool initClientSide();

protected:
    std::string m_address = "";
    uint16_t m_port = 0;
    sockaddr_in m_socketAddr;

    TcpSocket(const int socketFd);

    bool initSocket();
    long recvData(int socket,
                  void* bufferPosition,
                  const size_t bufferSize,
                  int flags);
    ssize_t sendData(int socket,
                     const void* bufferPosition,
                     const size_t bufferSize,
                     int flags);
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_SOCKET_H
