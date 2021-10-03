/**
 *  @file    unix_domain_socket_unix_domain_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_TEST_H
#define UNIX_DOMAIN_TEST_H

#include <libKitsunemimiCommon/test_helper/compare_test_helper.h>

namespace Kitsunemimi
{
struct DataBuffer;
namespace Network
{
class UnixDomainServer;
class UnixDomainSocket;

class UnixDomain_Test
        : public Kitsunemimi::CompareTestHelper
{
public:
    UnixDomain_Test();

    DataBuffer* m_buffer = nullptr;
    UnixDomainSocket* m_socketServerSide = nullptr;

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    UnixDomainServer* m_server = nullptr;
    UnixDomainSocket* m_socketClientSide = nullptr;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // UNIX_DOMAIN_TEST_H