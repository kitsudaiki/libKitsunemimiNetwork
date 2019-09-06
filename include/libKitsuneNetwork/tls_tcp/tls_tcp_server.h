/**
 *  @file    tls_tcp_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TLS_TCP_SERVER_H
#define TLS_TCP_SERVER_H

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <abstract_server.h>

namespace Kitsune
{
namespace Network
{

class TlsTcpServer : public AbstractServer
{
public:
    TlsTcpServer(const std::string certFile,
                 const std::string keyFile,
                 NetworkTrigger* trigger = nullptr);
    ~TlsTcpServer();

    bool initSocket(const uint16_t port);
    AbstractClient* waitForIncomingConnection();

private:
    struct sockaddr_in m_server;

    std::string m_caFile = "";
    std::string m_certFile = "";
    std::string m_keyFile = "";
};

} // namespace Network
} // namespace Kitsune

#endif // TLS_TCP_SERVER_H
