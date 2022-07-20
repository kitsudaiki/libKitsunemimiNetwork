/**
 *  @file    tcp_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tcp/tcp_server.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor
 *
 * @param port port-number where the server should be listen
 */
TcpServer::TcpServer(const uint16_t port)
{
    m_port = port;
    type = 2;
}

TcpServer::TcpServer() {}

/**
 * @brief destructor
 */
TcpServer::~TcpServer() {}

/**
 * @brief creates a server on a specific port
 *
 * @param error reference for error-output
 *
 * @return false, if server creation failed, else true
 */
bool
TcpServer::initServer(ErrorContainer &error)
{
    // create socket
    serverFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverFd < 0)
    {
        error.addMeesage("Failed to create a tcp-socket");
        error.addSolution("Maybe no permissions to create a tcp-server on the system");
        return false;
    }

    // make the port reusable
    int enable = 1;
    if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        error.addMeesage("Failed set socket-options for tcp-server on port: "
                         + std::to_string(m_port));
        return false;
    }

    // set server-settings
    memset(&m_server, 0, sizeof (m_server));
    m_server.sin_family = AF_INET;
    m_server.sin_addr.s_addr = htonl(INADDR_ANY);
    m_server.sin_port = htons(m_port);

    // bind to port
    if(bind(serverFd, reinterpret_cast<struct sockaddr*>(&m_server), sizeof(m_server)) < 0)
    {
        error.addMeesage("Failed to bind tcp-socket to port: " + std::to_string(m_port));
        return false;
    }

    // start listening for incoming connections
    if(listen(serverFd, 5) == -1)
    {
        error.addMeesage("Failed listen on tcp-socket on port: " + std::to_string(m_port));
        return false;
    }

    LOG_INFO("Successfully initialized tcp-socket server on port: " + std::to_string(m_port));

    return true;
}

} // namespace Network
} // namespace Kitsunemimi
