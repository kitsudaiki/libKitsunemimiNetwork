/**
 *  @file    tcp_client_tcp_server_test.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef TCPCLIENT_TCPSERVER_TEST_H
#define TCPCLIENT_TCPSERVER_TEST_H

#include <testing/test.h>

namespace Kitsune
{
namespace Network
{

class TcpServer;
class TcpClient;
class DummyBuffer;

class TcpClient_TcpServer_Test : public Kitsune::Common::Test
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

#endif // TCPCLIENT_TCPSERVER_TEST_H
