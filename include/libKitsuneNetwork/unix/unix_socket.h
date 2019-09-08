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

class UnixSocket : public AbstractSocket
{
public:
    UnixSocket(const std::string socketFile);
    UnixSocket(const int socketFd,
               sockaddr_un socket);

private:
    std::string m_socketFile = "";
    sockaddr_un m_socketAddr;

    bool initSocket();

    long recvData(int socket, void* bufferPosition, const size_t bufferSize, int flags);
    ssize_t sendData(int socket, const void* bufferPosition, const size_t bufferSize, int flags);
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_SOCKET_H
