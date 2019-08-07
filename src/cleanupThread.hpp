/**
 *  @file    cleanupThread.hpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef CLEANUPTHREAD_HPP
#define CLEANUPTHREAD_HPP

#include <queue>

#include <threading/commonThread.hpp>

namespace Kitsune
{
namespace Network
{
class TcpClient;

class CleanupThread : public Kitsune::CommonThread
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

#endif // CLEANUPTHREAD_HPP
