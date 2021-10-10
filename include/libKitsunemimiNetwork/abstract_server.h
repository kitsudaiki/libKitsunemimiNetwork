/**
 *  @file    abstract_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef ABSTRACT_SERVER_H
#define ABSTRACT_SERVER_H

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

namespace Kitsunemimi
{
struct RingBuffer;
namespace Network
{
class AbstractSocket;

class AbstractServer
        : public Kitsunemimi::Thread
{
public:
    enum serverTypes
    {
        UNDEFINED_TYPE = 0,
        UNIX_SERVER = 1,
        TCP_SERVER = 2,
        TLS_TCP_SERVER = 3
    };

    AbstractServer(void* target,
                   void (*processConnection)(void*, AbstractSocket*),
                   const std::string &threadName);
    ~AbstractServer();

    serverTypes getType();

    virtual void waitForIncomingConnection() = 0;
    bool closeServer();

protected:
    void run();

    int m_serverSocket = 0;
    serverTypes m_type = UNDEFINED_TYPE;

    // callback-parameter
    void* m_target = nullptr;
    void (*m_processConnection)(void*, AbstractSocket*);
};

} // namespace Network
} // namespace Kitsunemimi

#endif // ABSTRACT_SERVER_H
