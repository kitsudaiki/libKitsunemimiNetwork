/**
 *  @file    tcp_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <libKitsuneNetwork/abstract_server.h>

namespace Kitsune
{
namespace Network
{

class TcpServer : public AbstractServer
{
public:
    TcpServer(void* target,
              void (*processConnection)(void*, AbstractSocket*));
    ~TcpServer();

    bool initServer(const uint16_t port);
    AbstractSocket* waitForIncomingConnection();

protected:
    uint16_t m_port = 0;
    struct sockaddr_in m_server;
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_SERVER_H
