/**
 *  @file    tcp_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <libKitsunemimiNetwork/abstract_server.h>

namespace Kitsunemimi
{
namespace Network
{

class TcpServer
        : public AbstractServer
{
public:
    TcpServer(void* target,
              void (*processConnection)(void*, AbstractSocket*),
              const std::string &threadName);
    ~TcpServer();

    bool initServer(const uint16_t port, ErrorContainer &error);
    bool waitForIncomingConnection(ErrorContainer &error);

protected:
    uint16_t m_port = 0;
    struct sockaddr_in m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TCP_SERVER_H
