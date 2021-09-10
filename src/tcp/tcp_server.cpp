/**
 *  @file    tcp_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tcp/tcp_server.h>
#include <libKitsunemimiNetwork/tcp/tcp_socket.h>
#include <libKitsunemimiPersistence/logger/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor
 */
TcpServer::TcpServer(void* target,
                     void (*processConnection)(void*, AbstractSocket*))
    : AbstractServer(target,
                     processConnection)
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
 *
 * @return false, if server creation failed, else true
 */
bool
TcpServer::initServer(const uint16_t port)
{
    m_port = port;

    // create socket
    m_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_serverSocket < 0)
    {
        LOG_ERROR("Failed to create a tcp-socket");
        return false;
    }

    // make the port reusable
    int enable = 1;
    if(setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        LOG_ERROR("Failed set socket-options for tcp-server on port: " + std::to_string(m_port));
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
        LOG_ERROR("Failed to bind tcp-socket to port: " + std::to_string(m_port));
        return false;
    }

    // start listening for incoming connections
    if(listen(m_serverSocket, 5) == -1)
    {
        LOG_ERROR("Failed listen on tcp-socket on port: " + std::to_string(m_port));
        return false;
    }

    LOG_INFO("Successfully initialized tcp-socket server on port: " + std::to_string(m_port));

    return true;
}

/**
 * @brief wait for new incoming tcp-connections
 */
void
TcpServer::waitForIncomingConnection()
{
    uint32_t length = sizeof(struct sockaddr_in);

    //make new connection
    const int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);

    if(m_abort) {
        return;
    }

    if(fd < 0)
    {
        LOG_ERROR("Failed accept incoming connection on tcp-server with "
                  "port: " + std::to_string(m_port));
        return;
    }

    LOG_INFO("Successfully accepted incoming connection on tcp-socket server with "
                 "port : " + std::to_string(m_port));

    // create new socket-object from file-descriptor
    TcpSocket* tcpSocket = new TcpSocket(fd);
    m_processConnection(m_target, tcpSocket);
}

} // namespace Network
} // namespace Kitsunemimi
