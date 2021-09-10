/**
 *  @file    tls_tcp_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TLS_TCP_SERVER_H
#define TLS_TCP_SERVER_H

#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <libKitsunemimiNetwork/tcp/tcp_server.h>

namespace Kitsunemimi
{
namespace Network
{

class TlsTcpServer
        : public TcpServer
{
public:
    TlsTcpServer(void* target,
                 void (*processConnection)(void*, AbstractSocket*),
                 const std::string &certFile,
                 const std::string &keyFile,
                 const std::string &caFile = "");
    ~TlsTcpServer();

    void waitForIncomingConnection();

private:
    std::string m_caFile = "";
    std::string m_certFile = "";
    std::string m_keyFile = "";
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TLS_TCP_SERVER_H
