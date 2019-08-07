/**
 *  @file    tcpClient_tcpServer_test.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef TCPCLIENT_TCPSERVER_TEST_HPP
#define TCPCLIENT_TCPSERVER_TEST_HPP

#include <testing/commonTest.hpp>

namespace Kitsune
{
namespace Network
{

class TcpServer;
class TcpClient;
class DummyBuffer;

class TcpClient_TcpServer_Test : public Kitsune::CommonTest
{
public:
    TcpClient_TcpServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    TcpServer* m_server = nullptr;
    TcpClient* m_clientClientSide = nullptr;
    TcpClient* m_clientServerSide = nullptr;
    DummyBuffer* m_buffer = nullptr;
};

}
}

#endif // TCPCLIENT_TCPSERVER_TEST_HPP
