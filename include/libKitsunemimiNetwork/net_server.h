/**
 *  @file    abstract_server.h
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
        m_server.m_target = target;
        m_server.m_processConnection = processConnection;
    }

    NetServer(T&& server,
              void* target,
              void (*processConnection)(void*, NetSocket<TlsTcpSocket>*),
              const std::string &threadName)
        : Kitsunemimi::Thread(threadName)
    {
        m_server = std::move(server);
        m_server.m_target = target;
        m_server.m_processConnection = processConnection;
    }

    NetServer(T&& server,
              void* target,
              void (*processConnection)(void*, NetSocket<UnixDomainSocket>*),
              const std::string &threadName)
        : Kitsunemimi::Thread(threadName)
    {
        m_server = std::move(server);
        m_server.m_target = target;
        m_server.m_processConnection = processConnection;
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
            if(m_server.waitForIncomingConnection(&m_abort, error) == false)
            {
                LOG_ERROR(error);
                error._alreadyPrinted = false;
            }
        }
    };

    T m_server;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // NET_SERVER_H
