/**
 *  @file    unix_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#include <abstract_socket.h>

namespace Kitsune
{
namespace Network
{
class UnixServer;

class UnixSocket : public AbstractSocket
{
    friend class UnixServer;

public:
    UnixSocket(const std::string socketFile);

    bool initClientSide();

protected:
    std::string m_socketFile = "";
    sockaddr_un m_socketAddr;

    UnixSocket(const int socketFd);

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

#endif // UNIX_SOCKET_H
