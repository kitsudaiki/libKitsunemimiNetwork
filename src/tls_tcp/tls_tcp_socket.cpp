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
 * @param certFile path to certificate-file
 * @param keyFile path to key-file
 * @param caFile path to ca-file
 */
TlsTcpSocket::TlsTcpSocket(const std::string &address,
                           const uint16_t port,
                           const std::string &threadName,
                           const std::string &certFile,
                           const std::string &keyFile,
                           const std::string &caFile)
    : TcpSocket(address, port, threadName)
{
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_caFile = caFile;
    m_type = TLS_TCP_SOCKET;
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
                           const std::string &threadName,
                           const std::string &certFile,
                           const std::string &keyFile,
                           const std::string &caFile)
    : TcpSocket(socketFd, threadName)
{
    m_certFile = certFile;
    m_keyFile = keyFile;
    m_caFile = caFile;
    m_type = TLS_TCP_SOCKET;
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
 * @brief init socket on client-side
 *
 * @return true, if successful, else false
 */
bool
TlsTcpSocket::initClientSide()
{
    if(m_isConnected) {
        return true;
    }

    bool result = initSocket();
    if(result == false) {
        return false;
    }

    result = initOpenssl();
    if(result == false) {
        return false;
    }

    m_isConnected = true;
    LOG_INFO("Successfully initialized encrypted tcp-socket client to targe: " + m_address);

    return true;
}

/**
 * @brief init ssl and bind it to the file-descriptor
 */
bool
TlsTcpSocket::initOpenssl()
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
        ErrorContainer error;
        error.errorMessage = "Failed to create ssl-context object";
        LOG_ERROR(error);
        return false;
    }
    SSL_CTX_set_options(m_ctx, SSL_OP_SINGLE_DH_USE);

    // set certificate
    result = SSL_CTX_use_certificate_file(m_ctx, m_certFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0)
    {
        ErrorContainer error;
        error.errorMessage = "Failed to load certificate file for ssl-encrytion. File path: "
                             + m_certFile;
        error.possibleSolution = "check if file '" + m_certFile+ "' exist and "
                                 "contains a valid certificate";
        LOG_ERROR(error);
        return false;
    }

    // set key
    result = SSL_CTX_use_PrivateKey_file(m_ctx, m_keyFile.c_str(), SSL_FILETYPE_PEM);
    if(result <= 0)
    {
        ErrorContainer error;
        error.errorMessage = "Failed to load key file for ssl-encrytion. File path: " + m_keyFile;
        error.possibleSolution = "check if file '" + m_keyFile+ "' exist and "
                                 "contains a valid key";
        LOG_ERROR(error);
        return false;
    }

    // set CA-file if exist
    if(m_caFile != "")
    {
        result = SSL_CTX_load_verify_locations(m_ctx, m_caFile.c_str(), nullptr);
        if(result <= 0)
        {
            ErrorContainer error;
            error.errorMessage = "Failed to load CA file for ssl-encrytion. File path: " + m_caFile;
            error.possibleSolution = "check if file '" + m_caFile+ "' exist and "
                                     "contains a valid CA";
            LOG_ERROR(error);
            return false;
        }

        // set the verification depth to 1
        SSL_CTX_set_verify_depth(m_ctx, 1);
    }

    // init ssl-cennection
    m_ssl = SSL_new(m_ctx);
    if (m_ssl == nullptr)
    {
        ErrorContainer error;
        error.errorMessage = "Failed to ini ssl";
        error.possibleSolution = "(no solution known)";
        LOG_ERROR(error);
        return false;
    }
    SSL_set_fd(m_ssl, m_socket);

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
            ErrorContainer error;
            error.errorMessage = "Failed to perform ssl-handshake (client-side)";
            error.possibleSolution = "Maybe the server is only plain TCP-server "
                                     "or doesn't support TLS";
            LOG_ERROR(error);
            return false;
        }
    }
    else
    {
        // try to accept incoming ssl-connection
        int result = SSL_accept(m_ssl);
        if(result <= 0)
        {
            ErrorContainer error;
            error.errorMessage = "Failed to perform ssl-handshake (client-side)";
            error.possibleSolution = "Maybe the incoming client is only plain TCP-client "
                                     "or doesn't support TLS";
            LOG_ERROR(error);
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
