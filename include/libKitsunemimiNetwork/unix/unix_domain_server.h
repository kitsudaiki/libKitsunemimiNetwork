/**
 *  @file    unix_domain_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_SERVER_H
#define UNIX_DOMAIN_SERVER_H

#include <string>
#include <libKitsunemimiNetwork/abstract_server.h>

namespace Kitsunemimi
{
namespace Network
{
class UnixDomainSocket;

class UnixDomainServer
        : public AbstractServer
{
public:
    UnixDomainServer(void* target,
                     void (*processConnection)(void*, AbstractSocket*),
                     const std::string &threadName);
    ~UnixDomainServer();

    bool initServer(const std::string &socketFile, ErrorContainer &error);
    bool waitForIncomingConnection(ErrorContainer &error);

private:
    std::string m_socketFile = "";
    struct sockaddr_un m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // UNIX_DOMAIN_SERVER_H
