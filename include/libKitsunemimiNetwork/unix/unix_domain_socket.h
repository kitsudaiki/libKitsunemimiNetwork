/**
 *  @file    unix_domain_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_SOCKET_H
#define UNIX_DOMAIN_SOCKET_H

#include <libKitsunemimiNetwork/net_socket.h>

namespace Kitsunemimi
{
namespace Network
{

class UnixDomainSocket
{

public:
    UnixDomainSocket(const std::string &socketFile);
    UnixDomainSocket();
    ~UnixDomainSocket();

    std::string m_socketFile = "";
    sockaddr_un m_socketAddr;
    bool m_isConnected = false;
    bool m_isClientSide = false;
    int m_socketFd = 0;
    uint32_t m_type = 0;

    UnixDomainSocket(const int socketFd);

    bool initClientSide(ErrorContainer &error);
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

#endif // UNIX_DOMAIN_SOCKET_H
