/**
 *  @file    abstract_server.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef ABSTRACT_SERVER_H
#define ABSTRACT_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <threading/thread.h>

namespace Kitsune
{
namespace Network
{
class NetworkTrigger;
class AbstractClient;

class AbstractServer : public Kitsune::Common::Thread
{
public:
    AbstractServer();
    ~AbstractServer();

    virtual AbstractClient* waitForIncomingConnection() = 0;
    virtual bool closeServer() = 0;

    uint64_t getNumberOfSockets();
    AbstractClient* getSocket(const uint32_t pos);

    // trigger-control
    void addAdditionalTrigger(NetworkTrigger* trigger);
    void clearTrigger();

protected:
    void run();

    int m_serverSocket = 0;
    std::vector<NetworkTrigger*> m_trigger;
    std::vector<AbstractClient*> m_sockets;
};

} // namespace Network
} // namespace Kitsune

#endif // ABSTRACT_SERVER_H
