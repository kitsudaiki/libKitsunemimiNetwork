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

#include <threading/thread.h>

namespace Kitsune
{
namespace Network
{
class NetworkTrigger;
class AbstractSocket;

class AbstractServer : public Kitsune::Common::Thread
{
public:
    AbstractServer();
    ~AbstractServer();

    virtual AbstractSocket* waitForIncomingConnection() = 0;
    bool closeServer();

    uint64_t getNumberOfSockets();
    AbstractSocket* getSocket(const uint32_t pos);

    // trigger-control
    void addAdditionalTrigger(NetworkTrigger* trigger);
    void clearTrigger();

protected:
    void run();

    int m_serverSocket = 0;
    std::vector<NetworkTrigger*> m_trigger;
    std::vector<AbstractSocket*> m_sockets;
};

} // namespace Network
} // namespace Kitsune

#endif // ABSTRACT_SERVER_H
