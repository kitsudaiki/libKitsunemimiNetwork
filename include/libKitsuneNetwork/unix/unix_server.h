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
class UnixSocket;

class UnixServer : public AbstractServer
{
public:
    UnixServer(MessageTrigger* messageTrigger,
               ConnectionTrigger* connectionTrigger);
    ~UnixServer();

    bool initServer(const std::string socketFile);
    AbstractSocket* waitForIncomingConnection();

private:
    std::string m_socketFile = "";
    struct sockaddr_un m_server;
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_SERVER_H
