/**
 *  @file    unix_domain_server.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/unix/unix_domain_socket.h>
#include <libKitsunemimiNetwork/unix/unix_domain_server.h>
#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor
 */
UnixDomainServer::UnixDomainServer(void* target,
                                   void (*processConnection)(void*, AbstractSocket*),
                                   const std::string &threadName)
    : AbstractServer(target,
                     processConnection,
                     threadName)
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
        ErrorContainer error;
        error.errorMessage = "Failed to create a unix-server, "
                             "because the filename is longer then 100 characters: '"
                             + m_socketFile
                             + "'";
        error.possibleSolution = "use a shorter name";
        LOG_ERROR(error);
        return false;
    }

    // create socket
    m_serverSocket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(m_serverSocket < 0)
    {
        ErrorContainer error;
        error.errorMessage = "Failed to create a unix-socket";
        error.possibleSolution = "Maybe no permissions to create a unix-socket on the system";
        LOG_ERROR(error);
        return false;
    }

    unlink(m_socketFile.c_str());
    m_server.sun_family = AF_LOCAL;
    strncpy(m_server.sun_path, m_socketFile.c_str(), m_socketFile.size());
    m_server.sun_path[m_socketFile.size()] = '\0';

    // bind to port
    if(bind(m_serverSocket, reinterpret_cast<struct sockaddr*>(&m_server), sizeof(m_server)) < 0)
    {
        ErrorContainer error;
        error.errorMessage = "Failed to bind unix-socket to addresse: '" + m_socketFile + "'";
        error.possibleSolution = "(no solution known)";
        LOG_ERROR(error);
        return false;
    }

    // start listening for incoming connections
    if(listen(m_serverSocket, 5) == -1)
    {
        ErrorContainer error;
        error.errorMessage = "Failed listen on unix-socket on addresse: '" + m_socketFile + "'";
        error.possibleSolution = "(no solution known)";
        LOG_ERROR(error);
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
        ErrorContainer error;
        error.errorMessage = "Failed accept incoming connection on unix-server with address: '"
                             + m_socketFile
                             + "'";
        error.possibleSolution = "(no solution known)";
        return;
    }

    LOG_INFO("Successfully accepted incoming connection on unix-socket server with address: '"
             + m_socketFile
             + "'");

    // create new socket-object from file-descriptor
    const std::string name = getThreadName() + "_client";
    UnixDomainSocket* unixSocket = new UnixDomainSocket(fd, name);
    m_processConnection(m_target, unixSocket);
}

} // namespace Network
} // namespace Kitsunemimi

