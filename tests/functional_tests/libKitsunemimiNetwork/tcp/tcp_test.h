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
struct DataBuffer;
namespace Network
{
class TcpServer;
class TcpSocket;

template<class>
class NetSocket;

template<class>
class NetServer;

class Tcp_Test
        : public Kitsunemimi::CompareTestHelper
{
public:
    Tcp_Test();

    DataBuffer* m_buffer = nullptr;
    NetSocket<TcpSocket>* m_socketServerSide = nullptr;

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    NetServer<TcpServer>* m_server = nullptr;
    NetSocket<TcpSocket>* m_socketClientSide = nullptr;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TCPSOCKET_TCPSERVER_TEST_H
