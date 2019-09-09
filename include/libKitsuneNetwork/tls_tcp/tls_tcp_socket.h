/**
 *  @file    tls_tcp_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TLS_TCP_SOCKET_H
#define TLS_TCP_SOCKET_H

#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <tcp/tcp_socket.h>

namespace Kitsune
{
namespace Network
{

class TlsTcpSocket : public TcpSocket
{
public:
    TlsTcpSocket(const std::string address,
                 const uint16_t port,
                 const std::string certFile,
                 const std::string keyFile);
    TlsTcpSocket(const int socketFd,
                 const std::string certFile,
                 const std::string keyFile);
    ~TlsTcpSocket();

protected:
    SSL_CTX* m_ctx;
    SSL* m_ssl;
    std::string m_certFile = "";
    std::string m_keyFile = "";

    long recvData(int, void* bufferPosition, const size_t bufferSize, int);
    ssize_t sendData(int, const void* bufferPosition, const size_t bufferSize, int);

    bool initOpenssl();
    void cleanupOpenssl();
};

} // namespace Network
} // namespace Kitsune

#endif // TLS_TCP_SOCKET_H
