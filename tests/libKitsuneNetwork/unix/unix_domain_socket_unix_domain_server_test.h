/**
 *  @file    unix_domain_socket_unix_domain_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_DOMAIN_SOCKET_UNIX_DOMAIN_SERVER_TEST_H
#define UNIX_DOMAIN_SOCKET_UNIX_DOMAIN_SERVER_TEST_H

#include <testing/unit_test.h>

namespace Kitsune
{
namespace Network
{

class UnixDomainServer;
class UnixDomainSocket;
class DummyBuffer;
class IncomeTrigger;

class UnixDomainSocket_UnixDomainServer_Test
        : public Kitsune::Common::UnitTest
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
    DummyBuffer* m_buffer = nullptr;
    IncomeTrigger* m_incomeTrigger = nullptr;
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_DOMAIN_SOCKET_UNIX_DOMAIN_SERVER_TEST_H
