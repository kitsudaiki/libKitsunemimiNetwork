/**
 *  @file    tls_tcp_client_tls_tcp_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TLSTCPCLIENT_TLSTCPSERVER_TEST_H
#define TLSTCPCLIENT_TLSTCPSERVER_TEST_H

#include <testing/unit_test.h>

namespace Kitsune
{
namespace Network
{

class TlsTcpServer;
class TlsTcpClient;
class DummyBuffer;

class TlsTcpClient_TcpServer_Test : public Kitsune::Common::UnitTest
{
public:
    TlsTcpClient_TcpServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    TlsTcpServer* m_server = nullptr;
    TlsTcpClient* m_clientClientSide = nullptr;
    TlsTcpClient* m_clientServerSide = nullptr;
    DummyBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsune

#endif // TLSTCPCLIENT_TLSTCPSERVER_TEST_H
