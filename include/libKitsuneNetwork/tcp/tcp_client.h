/**
 *  @file    tcp_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <abstract_client.h>

namespace Kitsune
{
namespace Network
{

class TcpClient : public AbstractClient
{
public:
    TcpClient(const std::string address,
              const uint16_t port);
    TcpClient(const int clientFd,
              struct sockaddr_in client);

private:
    std::string m_address = "";
    uint16_t m_port = 0;
    sockaddr_in m_client;

    bool initClientSide();
    long recvData(int socket, void* bufferPosition, const size_t bufferSize, int flags);
    ssize_t sendData(int socket, const void* bufferPosition, const size_t bufferSize, int flags);
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_CLIENT_H
