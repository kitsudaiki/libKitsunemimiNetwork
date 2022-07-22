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

template<class>
class NetSocket;

template<class>
class NetServer;

class UnixDomain_Test
        : public Kitsunemimi::CompareTestHelper
{
public:
    UnixDomain_Test();

    DataBuffer* m_buffer = nullptr;
    NetSocket<UnixDomainSocket>* m_socketServerSide = nullptr;

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    NetServer<UnixDomainServer>* m_server = nullptr;
    NetSocket<UnixDomainSocket>* m_socketClientSide = nullptr;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // UNIX_DOMAIN_TEST_H
