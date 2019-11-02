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

#include <libKitsunemimiCommon/thread.h>

namespace Kitsunemimi
{
namespace Network
{
class AbstractSocket;

class CleanupThread : public Kitsunemimi::Common::Thread
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
} // namespace Kitsunemimi

#endif // CLEANUP_THREAD_H
