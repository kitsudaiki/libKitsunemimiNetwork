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
class AbstractSocket;

class CleanupThread : public Kitsune::Common::Thread
{
public:
    CleanupThread();

    void addSocketForCleanup(AbstractSocket *socket);

protected:
    void run();

private:
    std::queue<AbstractSocket*> m_cleanupQueue;
};

} // namespace Network
} // namespace Kitsune

#endif // CLEANUP_THREAD_H
