/**
 *  @file    tls_tcp_socket_tls_tcp_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef TLSTCPSOCKET_TLSTCPSERVER_TEST_H
#define TLSTCPSOCKET_TLSTCPSERVER_TEST_H

#include <libKitsunemimiCommon/test_helper/compare_test_helper.h>

namespace Kitsunemimi
{
struct DataBuffer;
namespace Network
{
class TlsTcpServer;
class TlsTcpSocket;

template<class>
class NetSocket;

template<class>
class NetServer;

class TlsTcp_Test
        : public Kitsunemimi::CompareTestHelper
{
public:
    TlsTcp_Test();

    DataBuffer* m_buffer = nullptr;
    NetSocket<TlsTcpSocket>* m_socketServerSide = nullptr;

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    NetServer<TlsTcpServer>* m_server = nullptr;
    NetSocket<TlsTcpSocket>* m_socketClientSide = nullptr;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TLSTCPSOCKET_TLSTCPSERVER_TEST_H
