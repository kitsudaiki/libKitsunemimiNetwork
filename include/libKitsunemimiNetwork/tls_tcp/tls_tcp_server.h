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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <libKitsunemimiNetwork/tcp/tcp_server.h>

namespace Kitsunemimi
{
namespace Network
{
class TlsTcpSocket;

template<class>
class NetSocket;

class TlsTcpServer
{
public:
    TlsTcpServer(const uint16_t port,
                 const std::string &certFile,
                 const std::string &keyFile,
                 const std::string &caFile = "");
    TlsTcpServer();
    ~TlsTcpServer();

    bool initServer(ErrorContainer &error);
    bool waitForIncomingConnection(bool* abort, ErrorContainer &error);

    std::string m_caFile = "";
    std::string m_certFile = "";
    std::string m_keyFile = "";

    int serverFd = 0;
    uint32_t type = 0;

    // callback-parameter for new incoming connections
    void* m_target = nullptr;
    void (*m_processConnection)(void*, NetSocket<TlsTcpSocket>*);

    uint16_t m_port = 0;
    struct sockaddr_in m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TLS_TCP_SERVER_H
