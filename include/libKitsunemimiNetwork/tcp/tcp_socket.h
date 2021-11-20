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

#include <libKitsunemimiNetwork/abstract_socket.h>

namespace Kitsunemimi
{
namespace Network
{
class TcpServer;

class TcpSocket
        : public AbstractSocket
{
    friend class TcpServer;

public:
    TcpSocket(const std::string &address,
              const uint16_t port,
              const std::string &threadName);
    ~TcpSocket();

    bool initClientSide(ErrorContainer &error);

protected:
    std::string m_address = "";
    uint16_t m_port = 0;
    sockaddr_in m_socketAddr;

    TcpSocket(const int socketFd,
              const std::string &threadName);

    bool initSocket(ErrorContainer &error);
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
} // namespace Kitsunemimi

#endif // TCP_SOCKET_H
