/**
 *  @file    tcp_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <abstract_server.h>

namespace Kitsune
{
namespace Network
{
class TcpClient;

class TcpServer : public AbstractServer
{
public:
    TcpServer(NetworkTrigger* trigger = nullptr);
    ~TcpServer();

    bool initSocket(const uint16_t port);
    AbstractClient* waitForIncomingConnection();

private:
    struct sockaddr_in m_server;
};

} // namespace Network
} // namespace Kitsune

#endif // TCP_SERVER_H
