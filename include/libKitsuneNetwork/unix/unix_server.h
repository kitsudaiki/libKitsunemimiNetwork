/**
 *  @file    unix_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_SERVER_H
#define UNIX_SERVER_H

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

private:
    struct sockaddr_un m_server;
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_SERVER_H
