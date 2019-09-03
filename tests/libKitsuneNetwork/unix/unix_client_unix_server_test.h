/**
 *  @file    unix_client_unix_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_CLIENT_UNIX_SERVER_TEST_H
#define UNIX_CLIENT_UNIX_SERVER_TEST_H

#include <testing/unit_test.h>

namespace Kitsune
{
namespace Network
{

class UnixServer;
class UnixClient;
class DummyBuffer;

class UnixClient_UnixServer_Test : public Kitsune::Common::UnitTest
{
public:
    UnixClient_UnixServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    UnixServer* m_server = nullptr;
    UnixClient* m_clientClientSide = nullptr;
    UnixClient* m_clientServerSide = nullptr;
    DummyBuffer* m_buffer = nullptr;
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_CLIENT_UNIX_SERVER_TEST_H
