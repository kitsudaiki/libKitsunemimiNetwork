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
class DataBuffer;
namespace Network
{
class TlsTcpServer;
class TlsTcpSocket;

class TlsTcpSocket_TlsTcpServer_Test
        : public Kitsunemimi::CompareTestHelper
{
public:
    TlsTcpSocket_TlsTcpServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    TlsTcpServer* m_server = nullptr;
    TlsTcpSocket* m_socketClientSide = nullptr;
    TlsTcpSocket* m_socketServerSide = nullptr;
    DataBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TLSTCPSOCKET_TLSTCPSERVER_TEST_H
