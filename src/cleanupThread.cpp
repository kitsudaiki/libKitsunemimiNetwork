/**
 *  @file    cleanupThread.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "cleanupThread.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <cinttypes>
#include <unistd.h>

#include <tcp/tcpClient.hpp>
#include <iostream>

namespace Kitsune
{
namespace Network
{

/**
 * constructor
 */
CleanupThread::CleanupThread() {}

/**
 * @brief CleanupThread::addClientForCleanup
 * @param client
 */
void
CleanupThread::addClientForCleanup(TcpClient *client)
{
    mutexLock();
    m_cleanupQueue.push(client);
    mutexUnlock();
}

/**
 * @brief CleanupThread::run
 */
void
CleanupThread::run()
{
    while(!m_abort)
    {
        sleepThread(100000);

        mutexLock();
        if(m_cleanupQueue.size() > 0)
        {
            TcpClient* client = m_cleanupQueue.front();
            m_cleanupQueue.pop();
            client->stop();
            delete client;
        }
        mutexUnlock();
    }
}

} // namespace Network
} // namespace Kitsune
