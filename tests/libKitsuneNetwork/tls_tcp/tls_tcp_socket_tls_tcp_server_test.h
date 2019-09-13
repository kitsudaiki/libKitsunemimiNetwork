/**
 *  @file    tls_tcp_socket_tls_tcp_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TLSTCPSOCKET_TLSTCPSERVER_TEST_H
#define TLSTCPSOCKET_TLSTCPSERVER_TEST_H

#include <testing/unit_test.h>

namespace Kitsune
{
namespace Network
{

class TlsTcpServer;
class TlsTcpSocket;
class DummyBuffer;

class TlsTcpSocket_TcpServer_Test : public Kitsune::Common::UnitTest
{
public:
    TlsTcpSocket_TcpServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    TlsTcpServer* m_server = nullptr;
    TlsTcpSocket* m_socketClientSide = nullptr;
    TlsTcpSocket* m_socketServerSide = nullptr;
    DummyBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsune

#endif // TLSTCPSOCKET_TLSTCPSERVER_TEST_H
