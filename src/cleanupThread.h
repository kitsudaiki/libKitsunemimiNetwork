#ifndef CLEANUPTHREAD_H
#define CLEANUPTHREAD_H

#include <queue>

#include <threading/commonThread.h>

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

}
}

#endif // CLEANUPTHREAD_H
