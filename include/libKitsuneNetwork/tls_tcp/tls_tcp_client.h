/**
 *  @file    tls_tcp_client.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TLS_TCP_CLIENT_H
#define TLS_TCP_CLIENT_H

#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <abstract_client.h>

namespace Kitsune
{
namespace Network
{

class TlsTcpClient : public AbstractClient
{
public:
    TlsTcpClient(const std::string address,
                 const uint16_t port,
                 const std::string certFile,
                 const std::string keyFile);
    TlsTcpClient(const int clientFd,
                 struct sockaddr_in client,
                 const std::string certFile,
                 const std::string keyFile);
    ~TlsTcpClient();

private:
    std::string m_address = "";
    uint16_t m_port = 0;
    sockaddr_in m_client;

    SSL_CTX* m_ctx;
    SSL* m_ssl;
    std::string m_certFile = "";
    std::string m_keyFile = "";

    bool initClientSide();
    long recvData(int, void* bufferPosition, const size_t bufferSize, int);
    ssize_t sendData(int, const void* bufferPosition, const size_t bufferSize, int);

    void initOpenssl();
    void cleanupOpenssl();
    bool createContext();
    bool configureContext(SSL_CTX* ctx);
};

} // namespace Network
} // namespace Kitsune

#endif // TLS_TCP_CLIENT_H
