/**
 *  @file    unix_socket_unix_server_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "unix_socket_unix_server_test.h"
#include <buffering/data_buffer.h>

#include <unix/unix_socket.h>
#include <unix/unix_server.h>
#include <dummy_buffer.h>
#include <income_trigger.h>

namespace Kitsune
{
namespace Network
{

UnixSocket_UnixServer_Test::UnixSocket_UnixServer_Test() :
    Kitsune::Common::UnitTest("UnixSocket_UnixServer_Test")
{
    initTestCase();
    checkConnectionInit();
    checkLittleDataTransfer();
    checkBigDataTransfer();
    cleanupTestCase();
}

/**
 * initTestCase
 */
void
UnixSocket_UnixServer_Test::initTestCase()
{
    m_buffer = new DummyBuffer();
    m_incomeTrigger = new IncomeTrigger();
    m_server = new UnixServer(m_buffer, m_incomeTrigger);
}

/**
 * checkConnectionInit
 */
void
UnixSocket_UnixServer_Test::checkConnectionInit()
{
    // init server
    UNITTEST(m_server->getType(), AbstractServer::UNIX_SERVER);
    UNITTEST(m_server->initServer("/tmp/sock.uds"), true);
    UNITTEST(m_server->start(), true);

    // init client
    m_socketClientSide = new UnixSocket("/tmp/sock.uds");
    UNITTEST(m_socketClientSide->initClientSide(), true);
    UNITTEST(m_socketClientSide->getType(), AbstractSocket::UNIX_SOCKET);

    usleep(10000);

    UNITTEST(m_server->getNumberOfSockets(), 1);

    if(m_server->getNumberOfSockets() == 1)
    {
        m_socketServerSide = static_cast<UnixSocket*>(m_server->getSocket(0));
        UNITTEST(m_socketServerSide->getType(), AbstractSocket::UNIX_SOCKET);
    }
}

/**
 * checkLittleDataTransfer
 */
void
UnixSocket_UnixServer_Test::checkLittleDataTransfer()
{
    usleep(10000);

    std::string sendMessage("poipoipoi");
    UNITTEST(m_socketClientSide->sendMessage(sendMessage), true);
    usleep(10000);
    UNITTEST(m_buffer->getNumberOfWrittenBytes(), 9);


    if(m_buffer->getNumberOfWrittenBytes() == 9)
    {
        Common::DataBuffer* buffer = m_buffer->getBuffer();
        uint64_t bufferSize = buffer->bufferPosition;
        char recvMessage[bufferSize];
        memcpy(recvMessage, buffer->data, bufferSize);
        UNITTEST(bufferSize, 9);
        UNITTEST(recvMessage[2], sendMessage.at(2));
        m_buffer->clearBuffer();
    }
}

/**
 * checkBigDataTransfer
 */
void
UnixSocket_UnixServer_Test::checkBigDataTransfer()
{
    std::string sendMessage = "poi";
    UNITTEST(m_socketClientSide->sendMessage(sendMessage), true);
    for(uint32_t i = 0; i < 99999; i++)
    {
        m_socketClientSide->sendMessage(sendMessage);
    }
    usleep(10000);
    uint64_t totalIncom = m_buffer->getNumberOfWrittenBytes();
    Common::DataBuffer* dataBuffer = m_buffer->getBuffer();
    UNITTEST(totalIncom, 300000);
    UNITTEST(dataBuffer->bufferPosition, 300000);
    uint32_t numberOfPois = 0;
    for(uint32_t i = 0; i < 300000; i=i+3)
    {
        uint8_t* dataBufferData = static_cast<uint8_t*>(dataBuffer->data);
        if(dataBufferData[i] == 'p'
                && dataBufferData[i+1] == 'o'
                && dataBufferData[i+2] == 'i')
        {
            numberOfPois++;
        }
    }
    UNITTEST(numberOfPois, 100000);
}

/**
 * cleanupTestCase
 */
void
UnixSocket_UnixServer_Test::cleanupTestCase()
{
    UNITTEST(m_socketServerSide->closeSocket(), true);
    m_socketServerSide->closeSocket();
    UNITTEST(m_server->closeServer(), true);

    delete m_server;
    delete m_buffer;
}

} // namespace Network
} // namespace Kitsune
