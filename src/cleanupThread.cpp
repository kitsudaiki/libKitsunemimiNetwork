#include "cleanupThread.h"

#include <tcp/tcpClient.h>

namespace Kitsune
{
namespace Network
{

CleanupThread::CleanupThread()
{

}

void
CleanupThread::addClientForCleanup(TcpClient *client)
{
    mutexLock();
    m_cleanupQueue.push(client);
    mutexUnlock();
}

void
CleanupThread::run()
{
    while(!m_abort)
    {
        sleepThread(100000);

        mutexLock();

        TcpClient* client = m_cleanupQueue.front();
        m_cleanupQueue.pop();
        delete client;

        mutexUnlock();
    }
}

}
}
