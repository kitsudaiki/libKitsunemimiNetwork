/**
 *  @file    net_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef NET_SERVER_H
#define NET_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <libKitsunemimiCommon/threading/thread.h>
#include <libKitsunemimiCommon/logger.h>

#include <libKitsunemimiNetwork/net_socket.h>
#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_server.h>
#include <libKitsunemimiNetwork/unix/unix_domain_server.h>
#include <libKitsunemimiNetwork/tcp/tcp_server.h>


namespace Kitsunemimi
{
struct RingBuffer;
namespace Network
{
class TcpSocket;
class UnixDomainSocket;
class TlsTcpSocket;

class TcpServer;
class UnixDomainServer;
class TlsTcpServer;

template<class>
class NetSocket;

template<class T>
class NetServer
        : public Kitsunemimi::Thread
{
public:

    /**
     * @brief constructor for tcp-typ-socket
     *
     * @param server base-server
     * @param target target-object for the callback
     * @param processConnection callback-function
     * @param threadName name of the thread of the server
     */
    NetServer(T&& server,
              void* target,
              void (*processConnection)(void*, NetSocket<TcpSocket>*),
              const std::string &threadName)
        : Kitsunemimi::Thread(threadName)
    {
        m_server = std::move(server);
        m_target = target;
        m_processTcpConnection = processConnection;
    }

    /**
     * @brief constructor for uds-typ-socket
     *
     * @param server base-server
     * @param target target-object for the callback
     * @param processConnection callback-function
     * @param threadName name of the thread of the server
     */
    NetServer(T&& server,
              void* target,
              void (*processConnection)(void*, NetSocket<UnixDomainSocket>*),
              const std::string &threadName)
        : Kitsunemimi::Thread(threadName)
    {
        m_server = std::move(server);
        m_target = target;
        m_processUnixDomainConnection = processConnection;
    }

    /**
     * @brief constructor for tls-typ-socket
     *
     * @param server base-server
     * @param target target-object for the callback
     * @param processConnection callback-function
     * @param threadName name of the thread of the server
     */
    NetServer(T&& server,
              void* target,
              void (*processConnection)(void*, NetSocket<TlsTcpSocket>*),
              const std::string &threadName)
        : Kitsunemimi::Thread(threadName)
    {
        m_server = std::move(server);
        m_target = target;
        m_processTlsTcpConnection = processConnection;
    }

    /**
     * @brief destructor
     */
    ~NetServer()
    {
        closeServer();
    }

    /**
     * @brief get type of the server
     *
     * @return type-id (1=UDS, 2=TCP, 3=TLS_TCP)
     */
    uint32_t getType()
    {
        return m_server.type;
    }

    /**
     * @brief initialize server
     *
     * @param error reference for error-output
     *
     * @return true, if successful, else false
     */
    bool initServer(ErrorContainer &error)
    {
        return m_server.initServer(error);
    }

    /**
     * @brief close-server
     *
     * @return true, if successful, else false
     */
    bool closeServer()
    {
        // precheck
        if(m_abort == true) {
            return false;
        }

        m_abort = true;

        // close server-socket
        if(m_server.getServerFd() >= 0)
        {
            // close server itself
            shutdown(m_server.getServerFd(), SHUT_RDWR);
            close(m_server.getServerFd());
            //m_server.serverFd = 0;
        }

        LOG_INFO("Successfully closed server");

        return true;
    }

protected:
    void run()
    {
        ErrorContainer error;
        while(m_abort == false)
        {
            if(waitForIncomingConnections(error) == false)
            {
                LOG_ERROR(error);
                error._alreadyPrinted = false;
            }
        }
    };

    /**
     * @brief wait for new incoming connection
     *
     * @param error reference for error-output
     *
     * @return true, if successful, else false
     */
    bool waitForIncomingConnections(Kitsunemimi::ErrorContainer &error)
    {
        uint32_t length = sizeof(struct sockaddr_in);

        //make new connection
        const int fd = accept(m_server.getServerFd(),
                              reinterpret_cast<struct sockaddr*>(&m_server.socketAddr),
                              &length);

        if(m_abort)
        {
            // TODO: close connection if fd > 0
            return true;
        }

        if(fd < 0)
        {
            error.addMeesage("Failed accept incoming connection on net-server");
            return false;
        }

        LOG_INFO("Successfully accepted incoming connection on net-server");

        // create new socket-object from file-descriptor
        if(std::is_same<T, TcpServer>::value)
        {
            const std::string name = "TCP_socket";
            TcpSocket tcpSocket(fd);
            NetSocket<TcpSocket>* netSocket =
                    new NetSocket<TcpSocket>(std::move(tcpSocket), name);
            netSocket->initConnection(error);
            m_processTcpConnection(m_target, netSocket);
        }
        else if(std::is_same<T, UnixDomainServer>::value)
        {
            const std::string name = "UDS_socket";
            UnixDomainSocket unixSocket(fd);
            NetSocket<UnixDomainSocket>* netSocket =
                    new NetSocket<UnixDomainSocket>(std::move(unixSocket), name);
            netSocket->initConnection(error);
            m_processUnixDomainConnection(m_target, netSocket);
        }
        else if(std::is_same<T, TlsTcpServer>::value)
        {
            const std::string name = "TLS_TCP_socket";
            TcpSocket tcpSocket(fd);
            TlsTcpSocket tlsTcpSocket(std::move(tcpSocket),
                                      m_server.certFile,
                                      m_server.keyFile,
                                      m_server.caFile);
            NetSocket<TlsTcpSocket>* netSocket =
                    new NetSocket<TlsTcpSocket>(std::move(tlsTcpSocket), name);
            netSocket->initConnection(error);
            m_processTlsTcpConnection(m_target, netSocket);
        }

        return true;
    }

    // callback-parameter for new incoming connections
    void* m_target = nullptr;
    void (*m_processTcpConnection)(void*, NetSocket<TcpSocket>*);
    void (*m_processUnixDomainConnection)(void*, NetSocket<UnixDomainSocket>*);
    void (*m_processTlsTcpConnection)(void*, NetSocket<TlsTcpSocket>*);

    T m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // NET_SERVER_H
