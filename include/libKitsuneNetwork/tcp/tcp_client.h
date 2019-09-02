/**
 *  @file    tcp_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <cinttypes>
#include <unistd.h>
#include <string>
#include <vector>

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
    ~TcpClient();

    // send
    bool sendMessage(const std::string &message);
    bool sendMessage(const uint8_t *message,
                     const uint64_t numberOfBytes);

    bool closeSocket();

private:
    std::string m_address = "";
    uint16_t m_port = 0;
    int m_clientSocket = 0;
    sockaddr_in m_client;

    bool initClientSide();
    bool waitForMessage();
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_CLIENT_H
