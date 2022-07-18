/**
 *  @file    tls_tcp_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_socket.h>
#include <libKitsunemimiCommon/threading/cleanup_thread.h>
#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor for the socket-side of the tcp-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 * @param threadName thread-name
 * @param certFile path to certificate-file
 * @param keyFile path to key-file
 * @param caFile path to ca-file
 */
TlsTcpSocket::TlsTcpSocket(const std::string &address,
                           const uint16_t port,
                           const std::string &certFile,
                           const std::string &keyFile,
                           const std::string &caFile)
{
    m_address = address;
    m_port = port;
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_caFile = caFile;
    m_type = 3;
}

TlsTcpSocket::TlsTcpSocket()
{

}

/**
 * @brief constructor for the server-side of the tcp-connection, which is called by the
 *        tcp-server for each incoming connection
 *
 * @param socketFd file-descriptor of the socket-socket
 * @param certFile path to certificate-file
 * @param keyFile path to key-file
 * @param caFile path to ca-file
 */
TlsTcpSocket::TlsTcpSocket(const int socketFd,
                           const std::string &certFile,
                           const std::string &keyFile,
                           const std::string &caFile)
{
    m_socketFd = socketFd;
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_caFile = caFile;
    m_type = 3;
    m_isConnected = true;
}

/**
 * @brief destructor
 */
TlsTcpSocket::~TlsTcpSocket()
{
    cleanupOpenssl();
}

/**
 * @brief init tcp-socket and connect to the server
 *
 * @param error reference for error-output
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
TlsTcpSocket::initSocket(ErrorContainer &error)
{
    struct sockaddr_in address;
    struct hostent* hostInfo;
    unsigned long addr;

    // create socket
    m_socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socketFd < 0)
    {
        error.addMeesage("Failed to create a tcp-socket");
        error.addSolution("Maybe no permissions to create a tcp-socket on the system");
        return false;
    }

    // set TCP_NODELAY for sure
    int optval = 1;
    if(setsockopt(m_socketFd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int)) < 0)
    {
        error.addMeesage("'setsockopt'-function failed");
        return false;
    }

    // set server-address
    memset(&address, 0, sizeof(address));
    if((addr = inet_addr(m_address.c_str())) != INADDR_NONE)
    {
        memcpy(reinterpret_cast<char*>(&address.sin_addr), &addr, sizeof(addr));
    }
    else
    {
        // get server-connection via host-name instead of ip-address
        hostInfo = gethostbyname(m_address.c_str());
        if(hostInfo == nullptr)
        {
            error.addMeesage("Failed to get host by address: " + m_address);
            error.addSolution("Check DNS, which is necessary to resolve the address");
            return false;
        }

        memcpy(reinterpret_cast<char*>(&address.sin_addr),
               hostInfo->h_addr,
               static_cast<size_t>(hostInfo->h_length));
    }

    // set other informations
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);

    // create connection
    if(connect(m_socketFd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0)
    {
        error.addMeesage("Failed to initialized tcp-socket client to target: " + m_address);
        error.addSolution("Check if the target-server is running and reable");
        return false;
    }

    m_socketAddr = address;

    return true;
}

/**
 * @brief init socket on client-side
 *
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
TlsTcpSocket::initClientSide(ErrorContainer &error)
{
    if(m_isConnected) {
        return true;
    }

    if(initSocket(error) == false) {
        return false;
    }

    if(initOpenssl(error) == false) {
        return false;
    }

    m_isConnected = true;
    LOG_INFO("Successfully initialized encrypted tcp-socket client to targe: " + m_address);

    return true;
}

/**
 * @brief init ssl and bind it to the file-descriptor
 *
 * @param error reference for error-output
 */
bool
TlsTcpSocket::initOpenssl(ErrorContainer &error)
{
    int result = 0;

    // common ssl-init
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    SSL_library_init();

    // set ssl-type
    const SSL_METHOD* method;
    if(m_isClientSide) {
        method = TLS_client_method();
    } else {
        method = TLS_server_method();
    }

    // init ssl-context
    m_ctx = SSL_CTX_new(method);
    if(m_ctx == nullptr)
    {
        error.addMeesage( "Failed to create ssl-context object");
        return false;
    }
    SSL_CTX_set_options(m_ctx, SSL_OP_SINGLE_DH_USE);

    // set certificate
    result = SSL_CTX_use_certificate_file(m_ctx, m_certFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0)
    {
        error.addMeesage("Failed to load certificate file for ssl-encrytion. File path: \""
                         + m_certFile
                         + "\"");
        error.addSolution("check if file \"" + m_certFile+ "\" exist and "
                           "contains a valid certificate");
        return false;
    }

    // set key
    result = SSL_CTX_use_PrivateKey_file(m_ctx, m_keyFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0)
    {
        error.addMeesage("Failed to load key file for ssl-encrytion. File path: " + m_keyFile);
        error.addSolution("check if file \"" + m_keyFile+ "\" exist and contains a valid key");
        return false;
    }

    // set CA-file if exist
    if(m_caFile != "")
    {
        result = SSL_CTX_load_verify_locations(m_ctx, m_caFile.c_str(), nullptr);
        if(result <= 0)
        {
            error.addMeesage("Failed to load CA file for ssl-encrytion. File path: " + m_caFile);
            error.addSolution("check if file \"" + m_caFile+ "\" exist and contains a valid CA");
            return false;
        }

        // set the verification depth to 1
        SSL_CTX_set_verify_depth(m_ctx, 1);
    }

    // init ssl-cennection
    m_ssl = SSL_new(m_ctx);
    if (m_ssl == nullptr)
    {
        error.addMeesage("Failed to ini ssl");
        return false;
    }
    SSL_set_fd(m_ssl, m_socketFd);

    // enable certificate validation, if ca-file was set
    if(m_caFile != "")
    {
        // SSL_VERIFY_PEER -> check cert if exist
        // SSL_VERIFY_FAIL_IF_NO_PEER_CERT -> server requires cert
        // SSL_VERIFY_CLIENT_ONCE -> check only on initial handshake
        SSL_set_verify(m_ssl,
                       SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT | SSL_VERIFY_CLIENT_ONCE,
                       nullptr);
    }

    // process tls-handshake
    if(m_isClientSide)
    {
        // try to connect to server
        result = SSL_connect(m_ssl);
        if(result <= 0)
        {
            error.addMeesage("Failed to perform ssl-handshake (client-side)");
            error.addSolution("Maybe the server is only plain TCP-server or doesn't support TLS");
            return false;
        }
    }
    else
    {
        // try to accept incoming ssl-connection
        int result = SSL_accept(m_ssl);
        if(result <= 0)
        {
            error.addMeesage("Failed to perform ssl-handshake (client-side)");
            error.addSolution("Maybe the incoming client is only plain TCP-client "
                              "or doesn't support TLS");
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
    if(m_ssl != nullptr)
    {
        SSL_shutdown(m_ssl);
        SSL_free(m_ssl);
    }

    if(m_ctx == nullptr) {
        SSL_CTX_free(m_ctx);
    }

    ERR_free_strings();
    EVP_cleanup();

    return true;
}

} // namespace Network
} // namespace Kitsunemimi
