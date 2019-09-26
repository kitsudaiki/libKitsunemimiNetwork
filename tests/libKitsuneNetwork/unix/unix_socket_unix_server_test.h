/**
 *  @file    unix_socket_unix_server_test.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef UNIX_SOCKET_UNIX_SERVER_TEST_H
#define UNIX_SOCKET_UNIX_SERVER_TEST_H

#include <testing/unit_test.h>

namespace Kitsune
{
namespace Network
{

class UnixServer;
class UnixSocket;
class DummyBuffer;
class IncomeTrigger;

class UnixSocket_UnixServer_Test : public Kitsune::Common::UnitTest
{
public:
    UnixSocket_UnixServer_Test();

private:
    void initTestCase();
    void checkConnectionInit();
    void checkLittleDataTransfer();
    void checkBigDataTransfer();
    void cleanupTestCase();

    UnixServer* m_server = nullptr;
    UnixSocket* m_socketClientSide = nullptr;
    UnixSocket* m_socketServerSide = nullptr;
    DummyBuffer* m_buffer = nullptr;
    IncomeTrigger* m_incomeTrigger = nullptr;
};

} // namespace Network
} // namespace Kitsune

#endif // UNIX_SOCKET_UNIX_SERVER_TEST_H
