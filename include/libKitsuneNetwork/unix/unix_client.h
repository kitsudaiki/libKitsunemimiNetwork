/**
 *  @file    unix_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_CLIENT_H
#define UNIX_CLIENT_H

#include <abstract_client.h>

namespace Kitsune
{
namespace Network
{

class UnixClient : public AbstractClient
{
public:
    UnixClient(const std::string socketFile);
    UnixClient(const int clientFd,
               sockaddr_un client);

private:
    std::string m_socketFile = "";
    sockaddr_un m_client;

    bool initClientSide();

    long recvData(int socket, void* bufferPosition, const size_t bufferSize, int flags);
    ssize_t sendData(int socket, const void* bufferPosition, const size_t bufferSize, int flags);
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_CLIENT_H
