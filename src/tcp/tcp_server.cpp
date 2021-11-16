/**
 *  @file    tcp_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tcp/tcp_server.h>
#include <libKitsunemimiNetwork/tcp/tcp_socket.h>
#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor
 */
TcpServer::TcpServer(void* target,
                     void (*processConnection)(void*, AbstractSocket*),
                     const std::string &threadName)
    : AbstractServer(target,
                     processConnection,
                     threadName)
{
    m_type = TCP_SERVER;
}

/**
 * @brief destructor
 */
TcpServer::~TcpServer()
{
    closeServer();
}

/**
 * @brief creates a server on a specific port
 *
 * @param port port-number where the server should be listen
 * @param error reference for error-output
 *
 * @return false, if server creation failed, else true
 */
bool
TcpServer::initServer(const uint16_t port, ErrorContainer &error)
{
    m_port = port;

    // create socket
    m_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_serverSocket < 0)
    {
        error.errorMessage = "Failed to create a tcp-socket";
        error.possibleSolution = "Maybe no permissions to create a tcp-server on the system";
        return false;
    }

    // make the port reusable
    int enable = 1;
    if(setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        error.errorMessage = "Failed set socket-options for tcp-server on port: "
                             + std::to_string(m_port);
        error.possibleSolution = "(no solution known)";
        return false;
    }

    // set server-settings
    memset(&m_server, 0, sizeof (m_server));
    m_server.sin_family = AF_INET;
    m_server.sin_addr.s_addr = htonl(INADDR_ANY);
    m_server.sin_port = htons(m_port);

    // bind to port
    if(bind(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), sizeof(m_server)) < 0)
    {
        error.errorMessage = "Failed to bind tcp-socket to port: " + std::to_string(m_port);
        error.possibleSolution = "(no solution known)";
        return false;
    }

    // start listening for incoming connections
    if(listen(m_serverSocket, 5) == -1)
    {
        error.errorMessage = "Failed listen on tcp-socket on port: " + std::to_string(m_port);
        error.possibleSolution = "(no solution known)";
        return false;
    }

    LOG_INFO("Successfully initialized tcp-socket server on port: " + std::to_string(m_port));

    return true;
}

/**
 * @brief wait for new incoming tcp-connections
 *
 * @param error reference for error-output
 */
bool
TcpServer::waitForIncomingConnection(ErrorContainer &error)
{
    uint32_t length = sizeof(struct sockaddr_in);

    //make new connection
    const int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);

    if(m_abort) {
        return true;
    }

    if(fd < 0)
    {
        error.errorMessage = "Failed accept incoming connection on tcp-server with port: "
                             + std::to_string(m_port);
        error.possibleSolution = "(no solution known)";
        return false;
    }

    LOG_INFO("Successfully accepted incoming connection on tcp-socket server with port : "
             + std::to_string(m_port));

    // create new socket-object from file-descriptor
    const std::string name = getThreadName() + "_client";
    TcpSocket* tcpSocket = new TcpSocket(fd, name);
    m_processConnection(m_target, tcpSocket);

    return true;
}

} // namespace Network
} // namespace Kitsunemimi
