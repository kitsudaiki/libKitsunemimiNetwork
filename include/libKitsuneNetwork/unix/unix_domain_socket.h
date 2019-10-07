/**
 *  @file    unix_domain_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_SOCKET_H
#define UNIX_DOMAIN_SOCKET_H

#include <libKitsuneNetwork/abstract_socket.h>

namespace Kitsune
{
namespace Network
{
class UnixDomainServer;

class UnixDomainSocket : public AbstractSocket
{
    friend class UnixDomainServer;

public:
    UnixDomainSocket(const std::string socketFile);

    bool initClientSide();

protected:
    std::string m_socketFile = "";
    sockaddr_un m_socketAddr;

    UnixDomainSocket(const int socketFd);

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

#endif // UNIX_DOMAIN_SOCKET_H
