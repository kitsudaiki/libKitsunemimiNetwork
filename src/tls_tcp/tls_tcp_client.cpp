/**
 *  @file    tls_tcp_client.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <tls_tcp/tls_tcp_client.h>
#include <iostream>
#include <network_trigger.h>
#include <cleanup_thread.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor for the client-side of the tcp-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 */
TlsTcpClient::TlsTcpClient(const std::string address,
                           const uint16_t port,
                           const std::string certFile,
                           const std::string keyFile)
    : AbstractClient ()
{
    m_address = address;
    m_port = port;
    m_clientSide = true;

    m_certFile = certFile;
    m_keyFile = keyFile;

    initClientSide();
}

/**
 * constructor for the server-side of the tcp-connection, which is called by the
 * tcp-server for each incoming connection
 *
 * @param clientFd file-descriptor of the client-socket
 * @param client address for the client
 */
TlsTcpClient::TlsTcpClient(const int clientFd,
                           sockaddr_in client,
                           const std::string certFile,
                           const std::string keyFile)
    : AbstractClient ()
{
    m_clientSocket = clientFd;
    m_client = client;
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

    SSL_set_fd(m_ssl, m_clientSocket);

    int ssl_err = SSL_accept(m_ssl);

    if(ssl_err <= 0)
    {
        ERR_print_errors_fp(stderr);
    }
}

/**
 * @brief TlsTcpClient::~TlsTcpClient
 */
TlsTcpClient::~TlsTcpClient()
{
    cleanupOpenssl();
}

/**
 * init tcp-socket and connect to the server
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
TlsTcpClient::initClientSide()
{
    struct sockaddr_in server;
    struct hostent* hostInfo;
    unsigned long addr;

    // init ssl
    initOpenssl();
    createContext();

    // create socket
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_clientSocket < 0) {
        return false;
    }

    // set TCP_NODELAY for sure
    int optval = 1;
    if(setsockopt(m_clientSocket, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int)) < 0) {
        printf("Cannot set TCP_NODELAY option on listen socket (%s)\n", strerror(errno));
    }

    // set server-address
    memset(&server, 0, sizeof(server));
    if((addr = inet_addr(m_address.c_str())) != INADDR_NONE) {
        memcpy((char*)&server.sin_addr, &addr, sizeof(addr));
    }
    else
    {
        // get server-connection via host-name instead of ip-address
        hostInfo = gethostbyname(m_address.c_str());
        if(hostInfo == nullptr) {
            return false;
        }
        memcpy((char*)&server.sin_addr, hostInfo->h_addr, hostInfo->h_length);
    }

    // set other informations
    server.sin_family = AF_INET;
    server.sin_port = htons(m_port);

    // create connection
    if(connect(m_clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        // TODO: correctly close socket
        m_clientSocket = 0;
        return false;
    }

    // finish ssl-init
    m_ssl = SSL_new(m_ctx);
    SSL_set_fd(m_ssl, m_clientSocket);

    int iResult = SSL_connect(m_ssl);

    if(SSL_accept(m_ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    return true;
}

/**
 * @brief TlsTcpClient::init_openssl
 */
void
TlsTcpClient::initOpenssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    SSL_library_init();
}

/**
 * @brief TlsTcpClient::create_context
 * @return
 */
bool
TlsTcpClient::createContext()
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
 * @brief TlsTcpClient::recvData
 *
 * @return
 */
long
TlsTcpClient::recvData(int, void* bufferPosition, const size_t bufferSize, int)
{
    return SSL_read(m_ssl, bufferPosition, static_cast<int>(bufferSize));
}

/**
 * @brief TlsTcpClient::sendData
 *
 * @return
 */
ssize_t
TlsTcpClient::sendData(int, const void* bufferPosition, const size_t bufferSize, int)
{
    return SSL_write(m_ssl, bufferPosition, static_cast<int>(bufferSize));
}

/**
 * @brief TlsTcpClient::cleanup_openssl
 */
void
TlsTcpClient::cleanupOpenssl()
{
    SSL_shutdown(m_ssl);
    SSL_free(m_ssl);
    SSL_CTX_free(m_ctx);

    ERR_free_strings();
    EVP_cleanup();
}

} // namespace Network
} // namespace Kitsune
