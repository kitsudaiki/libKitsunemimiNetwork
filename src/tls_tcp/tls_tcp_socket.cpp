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
    : AbstractSocket ()
{
    m_address = address;
    m_port = port;
    m_clientSide = true;

    m_certFile = certFile;
    m_keyFile = keyFile;

    initSocketSide();
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
    : AbstractSocket ()
{
    m_socket = socketFd;
    m_socketAddr = socket;
    m_clientSide = false;

    m_certFile = certFile;
    m_keyFile = keyFile;


    // init ssl
    initOpenssl();
    createContext();

    // finish ssl-init
    m_ssl = SSL_new(m_ctx);

    if (m_ssl == nullptr)
    {
        fprintf(stderr, "SSL_new() failed\n");
        exit(EXIT_FAILURE);
    }

    SSL_set_fd(m_ssl, m_socket);

    int ssl_err = SSL_accept(m_ssl);

    if(ssl_err <= 0)
    {
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
 * init tcp-socket and connect to the server
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
TlsTcpSocket::initSocketSide()
{
    struct sockaddr_in address;
    struct hostent* hostInfo;
    unsigned long addr;

    // init ssl
    initOpenssl();
    createContext();

    // create socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0) {
        return false;
    }

    // set TCP_NODELAY for sure
    int optval = 1;
    if(setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int)) < 0) {
        printf("Cannot set TCP_NODELAY option on listen socket (%s)\n", strerror(errno));
    }

    // set server-address
    memset(&address, 0, sizeof(address));
    if((addr = inet_addr(m_address.c_str())) != INADDR_NONE) {
        memcpy((char*)&address.sin_addr, &addr, sizeof(addr));
    }
    else
    {
        // get server-connection via host-name instead of ip-address
        hostInfo = gethostbyname(m_address.c_str());
        if(hostInfo == nullptr) {
            return false;
        }
        memcpy((char*)&address.sin_addr, hostInfo->h_addr, hostInfo->h_length);
    }

    // set other informations
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);

    // create connection
    if(connect(m_socket, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        // TODO: correctly close socket
        m_socket = 0;
        return false;
    }

    m_socketAddr = address;

    // finish ssl-init
    m_ssl = SSL_new(m_ctx);
    SSL_set_fd(m_ssl, m_socket);

    const int result = SSL_connect(m_ssl);

    if(result <= 0
            || SSL_accept(m_ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    return true;
}

/**
 * @brief TlsTcpSocket::init_openssl
 */
void
TlsTcpSocket::initOpenssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    SSL_library_init();
}

/**
 * @brief TlsTcpSocket::create_context
 * @return
 */
bool
TlsTcpSocket::createContext()
{
    const SSL_METHOD* method;
    if(m_clientSide) {
        method = TLS_client_method();
    } else {
        method = TLS_server_method();
    }

    m_ctx = SSL_CTX_new(method);
    if(!m_ctx)
    {
        fprintf (stderr, "SSL_CTX_new ERROR\n");
        ERR_print_errors_fp(stderr);
        return false;
    }

    SSL_CTX_set_options(m_ctx, SSL_OP_SINGLE_DH_USE);

    int result = SSL_CTX_use_certificate_file(m_ctx, m_certFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0) {
        return false;
    }

    result = SSL_CTX_use_PrivateKey_file(m_ctx, m_keyFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0) {
        return false;
    }

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
