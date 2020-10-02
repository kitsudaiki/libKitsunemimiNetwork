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

#include <libKitsunemimiCommon/threading/thread.h>

namespace Kitsunemimi
{
namespace Network
{
class AbstractSocket;

class CleanupThread : public Kitsunemimi::Thread
{
public:
    static CleanupThread* getInstance();

    void addSocketForCleanup(AbstractSocket *socket);

protected:
    void run();

private:
    CleanupThread();

    std::queue<AbstractSocket*> m_cleanupQueue;
    static CleanupThread* m_instance;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // CLEANUP_THREAD_H
