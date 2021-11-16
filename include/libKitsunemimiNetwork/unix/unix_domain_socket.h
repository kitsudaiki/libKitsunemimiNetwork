/**
 *  @file    unix_domain_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_SOCKET_H
#define UNIX_DOMAIN_SOCKET_H

#include <libKitsunemimiNetwork/abstract_socket.h>

namespace Kitsunemimi
{
namespace Network
{
class UnixDomainServer;

class UnixDomainSocket
        : public AbstractSocket
{
    friend class UnixDomainServer;

public:
    UnixDomainSocket(const std::string &socketFile,
                     const std::string &threadName);

    bool initClientSide(ErrorContainer &error);

protected:
    std::string m_socketFile = "";
    sockaddr_un m_socketAddr;

    UnixDomainSocket(const int socketFd,
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

#endif // UNIX_DOMAIN_SOCKET_H
