/**
 *  @file    unix_domain_socket_unix_domain_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_SOCKET_UNIX_DOMAIN_SERVER_TEST_H
#define UNIX_DOMAIN_SOCKET_UNIX_DOMAIN_SERVER_TEST_H

#include <libKitsunemimiCommon/test_helper/compare_test_helper.h>

namespace Kitsunemimi
{
struct DataBuffer;
namespace Network
{
class UnixDomainServer;
class UnixDomainSocket;

class UnixDomainSocket_UnixDomainServer_Test
        : public Kitsunemimi::CompareTestHelper
{
public:
    UnixDomainSocket_UnixDomainServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    UnixDomainServer* m_server = nullptr;
    UnixDomainSocket* m_socketClientSide = nullptr;
    UnixDomainSocket* m_socketServerSide = nullptr;
    DataBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // UNIX_DOMAIN_SOCKET_UNIX_DOMAIN_SERVER_TEST_H
