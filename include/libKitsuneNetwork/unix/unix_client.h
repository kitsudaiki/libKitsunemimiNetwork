/**
 *  @file    unix_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_CLIENT_H
#define UNIX_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

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
    ~UnixClient();

    // send
    bool sendMessage(const std::string &message);
    bool sendMessage(const uint8_t *message,
                     const uint64_t numberOfBytes);

    bool closeSocket();

private:
    std::string m_socketFile = "";
    uint16_t m_port = 0;
    int m_clientSocket = 0;
    sockaddr_un m_client;

    bool initClientSide();
    bool waitForMessage();
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_CLIENT_H
