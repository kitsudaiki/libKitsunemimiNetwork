/**
 *  @file    unix_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_SERVER_H
#define UNIX_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <abstract_server.h>

namespace Kitsune
{
namespace Network
{
class UnixClient;

class UnixServer : public AbstractServer
{
public:
    UnixServer(NetworkTrigger* trigger = nullptr);
    ~UnixServer();

    bool initSocket(const std::string socketFile);
    AbstractClient* waitForIncomingConnection();
    bool closeServer();

private:
    struct sockaddr_un m_server;
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_SERVER_H
