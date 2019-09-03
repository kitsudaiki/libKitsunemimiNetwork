/**
 *  @file    unix_client_unix_server_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "unix_client_unix_server_test.h"
#include <buffering/data_buffer.h>

#include <unix/unix_client.h>
#include <unix/unix_server.h>
#include <dummy_buffer.h>

namespace Kitsune
{
namespace Network
{

UnixClient_UnixServer_Test::UnixClient_UnixServer_Test() :
    Kitsune::Common::UnitTest("UnixClient_UnixServer_Test")
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
UnixClient_UnixServer_Test::initTestCase()
{
    m_buffer = new DummyBuffer();
    m_server = new UnixServer(m_buffer);
}

/**
 * checkConnectionInit
 */
void
UnixClient_UnixServer_Test::checkConnectionInit()
{
    UNITTEST(m_server->initSocket("/tmp/sock.uds"), true);
    UNITTEST(m_server->start(), true);
    m_clientClientSide = new UnixClient("/tmp/sock.uds");
    usleep(10000);

    UNITTEST(m_server->getNumberOfSockets(), 1);

    if(m_server->getNumberOfSockets() == 1)
    {
        m_clientServerSide = static_cast<UnixClient*>(m_server->getSocket(0));
    }
}

/**
 * checkLittleDataTransfer
 */
void
UnixClient_UnixServer_Test::checkLittleDataTransfer()
{
    usleep(10000);

    std::string sendMessage("poipoipoi");
    UNITTEST(m_clientClientSide->sendMessage(sendMessage), true);
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
UnixClient_UnixServer_Test::checkBigDataTransfer()
{
    std::string sendMessage = "poi";
    UNITTEST(m_clientClientSide->sendMessage(sendMessage), true);
    for(uint32_t i = 0; i < 99999; i++)
    {
        m_clientClientSide->sendMessage(sendMessage);
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
UnixClient_UnixServer_Test::cleanupTestCase()
{
    UNITTEST(m_clientServerSide->closeSocket(), true);
    m_clientServerSide->closeSocket();
    UNITTEST(m_server->closeServer(), true);

    delete m_server;
    delete m_buffer;
}

} // namespace Network
} // namespace Kitsune
