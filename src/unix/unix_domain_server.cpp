/**
 *  @file    unix_domain_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/unix/unix_domain_socket.h>
#include <libKitsunemimiNetwork/unix/unix_domain_server.h>
#include <libKitsunemimiPersistence/logger/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor
 */
UnixDomainServer::UnixDomainServer(void* target,
                                   void (*processConnection)(void*, AbstractSocket*))
    : AbstractServer(target,
                     processConnection)
{
    m_type = UNIX_SERVER;
}

/**
 * @brief destructor
 */
UnixDomainServer::~UnixDomainServer()
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
UnixDomainServer::initServer(const std::string &socketFile)
{
    m_socketFile = socketFile;

    // check file-path length to avoid conflics, when copy to the sockaddr_un-object
    if(m_socketFile.size() > 100)
    {
        LOG_ERROR("Failed to create a unix-socket, "
                  "because the filename is longer then 100 characters: " + m_socketFile);
        return false;
    }

    // create socket
    m_serverSocket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(m_serverSocket < 0)
    {
        LOG_ERROR("Failed to create a unix-socket");
        return false;
    }

    unlink(m_socketFile.c_str());
    m_server.sun_family = AF_LOCAL;
    strncpy(m_server.sun_path, m_socketFile.c_str(), m_socketFile.size());
    m_server.sun_path[m_socketFile.size()] = '\0';

    // bind to port
    if(bind(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), sizeof(m_server)) < 0)
    {
        LOG_ERROR("Failed to bind unix-socket to addresse: " + m_socketFile);
        return false;
    }

    // start listening for incoming connections
    if(listen(m_serverSocket, 5) == -1)
    {
        LOG_ERROR("Failed listen on unix-socket on addresse: " + m_socketFile);
        return false;
    }

    LOG_INFO("Successfully initialized unix-socket server on targe: " + m_socketFile);

    return true;
}

/**
 * @brief wait for new incoming unix-socket-connections
 */
void
UnixDomainServer::waitForIncomingConnection()
{
    uint32_t length = sizeof(struct sockaddr_un);

    //make new connection
    const int fd = accept(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), &length);

    if(m_abort) {
        return;
    }

    if(fd < 0)
    {
        LOG_ERROR("Failed accept incoming connection on unix-server with address: " + m_socketFile);
        return;
    }

    LOG_INFO("Successfully accepted incoming connection on unix-socket server with "
                 "address : " + m_socketFile);

    // create new socket-object from file-descriptor
    UnixDomainSocket* unixSocket = new UnixDomainSocket(fd);
    m_processConnection(m_target, unixSocket);
}

} // namespace Network
} // namespace Kitsunemimi

