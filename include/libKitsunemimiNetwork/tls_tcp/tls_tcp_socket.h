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

#include <libKitsunemimiNetwork/tcp/tcp_socket.h>

namespace Kitsunemimi
{
namespace Network
{

class TlsTcpSocket
{
public:
    TlsTcpSocket(const std::string &address,
                 const uint16_t port,
                 const std::string &certFile,
                 const std::string &keyFile,
                 const std::string &caFile = "");
    TlsTcpSocket();
    ~TlsTcpSocket();

    std::string m_address = "";
    uint16_t m_port = 0;
    SSL_CTX* m_ctx;
    SSL* m_ssl;
    sockaddr_in m_socketAddr;
    std::string m_certFile = "";
    std::string m_keyFile = "";
    std::string m_caFile = "";
    bool m_isConnected = false;
    bool m_isClientSide = false;
    int m_socketFd = 0;
    uint32_t m_type = 0;

    bool initOpenssl(ErrorContainer &error);
    bool initClientSide(ErrorContainer &error);
    bool initSocket(ErrorContainer &error);

    TlsTcpSocket(const int socketFd,
                 const std::string &certFile,
                 const std::string &keyFile,
                 const std::string &caFile="");

    long recvData(int,
                  void* bufferPosition,
                  const size_t bufferSize,
                  int);
    ssize_t sendData(int,
                     const void* bufferPosition,
                     const size_t bufferSize,
                     int);

    bool cleanupOpenssl();
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TLS_TCP_SOCKET_H
