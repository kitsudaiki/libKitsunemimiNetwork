/**
 *  @file    tls_tcp_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <tls_tcp/tls_tcp_socket.h>
#include <iostream>
#include <network_trigger.h>
#include <cleanup_thread.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor for the socket-side of the tcp-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 */
TlsTcpSocket::TlsTcpSocket(const std::string address,
                           const uint16_t port,
                           const std::string certFile,
                           const std::string keyFile)
    : TcpSocket(address, port)
{
    m_certFile = certFile;
    m_keyFile = keyFile;

    // init ssl
    initOpenssl();

    // try to connect to server
    const int result = SSL_connect(m_ssl);
    if(result <= 0) {
        ERR_print_errors_fp(stderr);
    }

}

/**
 * constructor for the server-side of the tcp-connection, which is called by the
 * tcp-server for each incoming connection
 *
 * @param socketFd file-descriptor of the socket-socket
 * @param socket address for the socket
 */
TlsTcpSocket::TlsTcpSocket(const int socketFd,
                           sockaddr_in socket,
                           const std::string certFile,
                           const std::string keyFile)
    : TcpSocket(socketFd, socket)
{
    m_certFile = certFile;
    m_keyFile = keyFile;

    // init ssl
    initOpenssl();

    // try to accept incoming ssl-connection
    int result = SSL_accept(m_ssl);
    if(result <= 0) {
        ERR_print_errors_fp(stderr);
    }
}

/**
 * @brief TlsTcpSocket::~TlsTcpSocket
 */
TlsTcpSocket::~TlsTcpSocket()
{
    cleanupOpenssl();
}

/**
 * @brief init ssl and bind it to the file-descriptor
 */
bool
TlsTcpSocket::initOpenssl()
{
    // common ssl-init
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    SSL_library_init();

    // set ssl-type
    const SSL_METHOD* method;
    if(m_clientSide) {
        method = TLS_client_method();
    } else {
        method = TLS_server_method();
    }

    // init ssl-context
    m_ctx = SSL_CTX_new(method);
    if(!m_ctx)
    {
        fprintf (stderr, "SSL_CTX_new ERROR\n");
        ERR_print_errors_fp(stderr);
        return false;
    }
    SSL_CTX_set_options(m_ctx, SSL_OP_SINGLE_DH_USE);

    // set certificate
    int result = SSL_CTX_use_certificate_file(m_ctx, m_certFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0) {
        return false;
    }

    // set key
    result = SSL_CTX_use_PrivateKey_file(m_ctx, m_keyFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0) {
        return false;
    }

    // init ssl-cennection
    m_ssl = SSL_new(m_ctx);
    if (m_ssl == nullptr)
    {
        fprintf(stderr, "SSL_new() failed\n");
        return false;
    }
    SSL_set_fd(m_ssl, m_socket);

    return true;
}

/**
 * @brief TlsTcpSocket::recvData
 *
 * @return
 */
long
TlsTcpSocket::recvData(int, void* bufferPosition, const size_t bufferSize, int)
{
    return SSL_read(m_ssl, bufferPosition, static_cast<int>(bufferSize));
}

/**
 * @brief TlsTcpSocket::sendData
 *
 * @return
 */
ssize_t
TlsTcpSocket::sendData(int, const void* bufferPosition, const size_t bufferSize, int)
{
    return SSL_write(m_ssl, bufferPosition, static_cast<int>(bufferSize));
}

/**
 * @brief TlsTcpSocket::cleanup_openssl
 */
void
TlsTcpSocket::cleanupOpenssl()
{
    SSL_shutdown(m_ssl);
    SSL_free(m_ssl);
    SSL_CTX_free(m_ctx);

    ERR_free_strings();
    EVP_cleanup();
}

} // namespace Network
} // namespace Kitsune
