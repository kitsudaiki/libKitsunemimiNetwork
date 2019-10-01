/**
 *  @file    tcp_socket_tcp_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCPSOCKET_TCPSERVER_TEST_H
#define TCPSOCKET_TCPSERVER_TEST_H

#include <testing/unit_test.h>

namespace Kitsune
{
namespace Common {
class DataBuffer;
}
namespace Network
{
class TcpServer;
class TcpSocket;
class MessageRingBuffer;
class AbstractSocket;

class TcpSocket_TcpServer_Test : public Kitsune::Common::UnitTest
{
public:
    TcpSocket_TcpServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    TcpServer* m_server = nullptr;
    TcpSocket* m_socketClientSide = nullptr;
    TcpSocket* m_socketServerSide = nullptr;
    Common::DataBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsune

#endif // TCPSOCKET_TCPSERVER_TEST_H
