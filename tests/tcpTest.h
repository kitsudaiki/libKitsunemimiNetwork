#ifndef TCPTEST_H
#define TCPTEST_H

#include <commonTest.h>

namespace Kitsune
{
namespace Network
{

class TcpServer;
class TcpClient;

class TcpTest : public Kitsune::CommonTest
{
public:
    TcpTest();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    TcpServer* m_server = nullptr;
    TcpClient* m_clientClientSide = nullptr;
    TcpClient* m_clientServerSide = nullptr;
};

}
}

#endif // TCPTEST_H
