/**
 *  @file    tcpTest.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef TCPTEST_H
#define TCPTEST_H

#include <testing/commonTest.hpp>

namespace Kitsune
{
namespace Network
{

class TcpServer;
class TcpClient;
class TestBuffer;

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
    TestBuffer* m_buffer = nullptr;
};

}
}

#endif // TCPTEST_H
