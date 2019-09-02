/**
 *  @file    cleanup_thread.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef CLEANUP_THREAD_H
#define CLEANUP_THREAD_H

#include <queue>

#include <threading/thread.h>

namespace Kitsune
{
namespace Network
{
class TcpClient;

class CleanupThread : public Kitsune::Common::Thread
{
public:
    CleanupThread();

    void addClientForCleanup(TcpClient* client);

protected:
    void run();

private:
    std::queue<TcpClient*> m_cleanupQueue;
};

} // namespace Network
} // namespace Kitsune

#endif // CLEANUP_THREAD_H
