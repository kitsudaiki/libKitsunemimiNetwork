/**
 *  @file    tls_tcp_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <tls_tcp/tls_tcp_socket.h>
#include <message_trigger.h>
#include <cleanup_thread.h>
#include <logger/logger.h>

using namespace Kitsune::Persistence;

namespace Kitsune
{
namespace Network
{

/**
 * constructor for the socket-side of the tcp-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 * @param certFile path to certificate-file
 * @param keyFile path to key-file
 */
TlsTcpSocket::TlsTcpSocket(const std::string address,
                           const uint16_t port,
                           const std::string certFile,
                           const std::string keyFile)
    : TcpSocket(address, port)
{
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_type = TLS_TCP_SOCKET;
}

/**
 * constructor for the server-side of the tcp-connection, which is called by the
 * tcp-server for each incoming connection
 *
 * @param socketFd file-descriptor of the socket-socket
 * @param certFile path to certificate-file
 * @param keyFile path to key-file
 */
TlsTcpSocket::TlsTcpSocket(const int socketFd,
                           const std::string certFile,
                           const std::string keyFile)
    : TcpSocket(socketFd)
{
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_type = TLS_TCP_SOCKET;
}

/**
 * @brief destructor
 */
TlsTcpSocket::~TlsTcpSocket()
{
    cleanupOpenssl();
}

/**
 * @brief init socket on client-side
 *
 * @return true, if successful, else false
 */
bool
TlsTcpSocket::initClientSide()
{
    bool result = initSocket();
    if(result == false) {
        return false;
    }

    result = initOpenssl();
    if(result == false) {
        return false;
    }

    LOG_info("Successfully initialized encrypted tcp-socket client to targe: " + m_address);

    return true;
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
        LOG_error("Failed to create ssl-context object");
        return false;
    }
    SSL_CTX_set_options(m_ctx, SSL_OP_SINGLE_DH_USE);

    // set certificate
    int result = SSL_CTX_use_certificate_file(m_ctx, m_certFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0)
    {
        LOG_error("Failed to load certificate file for ssl-encrytion. File path: " + m_certFile);
        return false;
    }

    // set key
    result = SSL_CTX_use_PrivateKey_file(m_ctx, m_keyFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0)
    {
        LOG_error("Failed to load key file for ssl-encrytion. File path: " + m_certFile);
        return false;
    }

    // init ssl-cennection
    m_ssl = SSL_new(m_ctx);
    if (m_ssl == nullptr)
    {
        LOG_error("Failed to ini ssl");
        return false;
    }
    SSL_set_fd(m_ssl, m_socket);

    // process tls-handshake
    if(m_clientSide)
    {
        // try to connect to server
        result = SSL_connect(m_ssl);
        if(result <= 0)
        {
            LOG_error("Failed to perform ssl-handshake (client-side)");
            return false;
        }
    }
    else
    {
        // try to accept incoming ssl-connection
        int result = SSL_accept(m_ssl);
        if(result <= 0)
        {
            LOG_error("Failed to perform ssl-handshake (server-side)");
            return false;
        }
    }

    return true;
}

/**
 * @brief receive data
 *
 * @return number of read bytes
 */
long
TlsTcpSocket::recvData(int,
                       void* bufferPosition,
                       const size_t bufferSize,
                       int)
{
    return SSL_read(m_ssl, bufferPosition, static_cast<int>(bufferSize));
}

/**
 * @brief send data
 *
 * @return number of written bytes
 */
ssize_t
TlsTcpSocket::sendData(int,
                       const void* bufferPosition,
                       const size_t bufferSize,
                       int)
{
    return SSL_write(m_ssl, bufferPosition, static_cast<int>(bufferSize));
}

/**
 * @brief cleanup openssl
 */
bool
TlsTcpSocket::cleanupOpenssl()
{
    if(m_ssl == nullptr
            || m_ctx == nullptr)
    {
        LOG_error("Failed to perform ssl-handshake (server-side)");
        return false;
    }

    SSL_shutdown(m_ssl);
    SSL_free(m_ssl);
    SSL_CTX_free(m_ctx);

    ERR_free_strings();
    EVP_cleanup();

    return true;
}

} // namespace Network
} // namespace Kitsune
