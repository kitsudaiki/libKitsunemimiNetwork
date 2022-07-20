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

#include <libKitsunemimiNetwork/tcp/tcp_server.h>
#include <libKitsunemimiNetwork/unix/unix_domain_server.h>
#include <libKitsunemimiNetwork/net_socket.h>

namespace Kitsunemimi
{
struct RingBuffer;
namespace Network
{
class TcpSocket;
class TlsTcpSocket;
class UnixDomainSocket;

template<class>
class NetSocket;

template<class T>
class NetServer
        : public Kitsunemimi::Thread
{
public:
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

    ~NetServer()
    {
        closeServer();
    }

    uint32_t getType()
    {
        return m_server.type;
    }

    bool closeServer()
    {
        // precheck
        if(m_abort == true) {
            return false;
        }

        m_abort = true;

        // close server-socket
        if(m_server.serverFd >= 0)
        {
            // close server itself
            shutdown(m_server.serverFd, SHUT_RDWR);
            close(m_server.serverFd);
            m_server.serverFd = 0;
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

    bool waitForIncomingConnections(Kitsunemimi::ErrorContainer &error)
    {
        uint32_t length = sizeof(struct sockaddr_in);

        //make new connection
        const int fd = accept(m_server.serverFd,
                              reinterpret_cast<struct sockaddr*>(&m_server.m_server),
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
            m_processTcpConnection(m_target, netSocket);
        }
        else if(std::is_same<T, UnixDomainServer>::value)
        {
            const std::string name = "UDS_socket";
            UnixDomainSocket unixSocket(fd);
            NetSocket<UnixDomainSocket>* netSocket =
                    new NetSocket<UnixDomainSocket>(std::move(unixSocket), name);
            m_processUnixDomainConnection(m_target, netSocket);
        }
    }

    // callback-parameter for new incoming connections
    void* m_target = nullptr;
    void (*m_processTcpConnection)(void*, NetSocket<TcpSocket>*);
    void (*m_processUnixDomainConnection)(void*, NetSocket<UnixDomainSocket>*);

    T m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // NET_SERVER_H
