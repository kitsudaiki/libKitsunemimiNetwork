/**
 *  @file    cleanup_thread.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "cleanup_thread.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <cinttypes>
#include <unistd.h>

#include <tcp/tcp_socket.h>
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
 * @brief CleanupThread::addSocketForCleanup
 * @param socket
 */
void
CleanupThread::addSocketForCleanup(AbstractSocket *socket)
{
    mutexLock();
    m_cleanupQueue.push(socket);
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
            AbstractSocket* socket = m_cleanupQueue.front();
            m_cleanupQueue.pop();
            socket->stop();
            delete socket;
        }
        mutexUnlock();
    }
}

} // namespace Network
} // namespace Kitsune
