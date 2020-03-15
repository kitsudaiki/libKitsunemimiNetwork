/**
 *  @file    tcp_socket_tcp_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TCPSOCKET_TCPSERVER_TEST_H
#define TCPSOCKET_TCPSERVER_TEST_H

#include <libKitsunemimiCommon/test_helper/compare_test_helper.h>

namespace Kitsunemimi
{
struct RingBuffer;
class DataBuffer;
namespace Network
{
class TcpServer;
class TcpSocket;
class AbstractSocket;

class TcpSocket_TcpServer_Test
        : public Kitsunemimi::CompareTestHelper
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
    DataBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TCPSOCKET_TCPSERVER_TEST_H
