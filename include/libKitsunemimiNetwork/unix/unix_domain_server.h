/**
 *  @file    unix_domain_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_SERVER_H
#define UNIX_DOMAIN_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <libKitsunemimiCommon/logger.h>
#include <libKitsunemimiNetwork/unix/unix_domain_socket.h>

namespace Kitsunemimi
{
namespace Network
{
template<class>
class NetSocket;

class UnixDomainServer
{
public:
    UnixDomainServer(const std::string &socketFile);
    UnixDomainServer();
    ~UnixDomainServer();

    bool initServer(ErrorContainer &error);

    int serverFd = 0;
    uint32_t type = 0;
    std::string m_socketFile = "";

    // callback-parameter for new incoming connections
    void* m_target = nullptr;
    void (*m_processConnection)(void*, NetSocket<UnixDomainSocket>*);

    uint16_t m_port = 0;
    struct sockaddr_un m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // UNIX_DOMAIN_SERVER_H
