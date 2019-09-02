/**
 *  @file    tcp_client_tcp_server_test.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "tcp_client_tcp_server_test.h"
#include <buffering/data_buffer.h>

#include <tcp/tcp_server.h>
#include <tcp/tcp_client.h>
#include <dummy_buffer.h>

namespace Kitsune
{
namespace Network
{

TcpClient_TcpServer_Test::TcpClient_TcpServer_Test() :
    Kitsune::Common::UnitTest("TcpClient_TcpServer_Test")
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
TcpClient_TcpServer_Test::initTestCase()
{
    m_buffer = new DummyBuffer();
    m_server = new TcpServer(m_buffer);
}

/**
 * checkConnectionInit
 */
void
TcpClient_TcpServer_Test::checkConnectionInit()
{
    UNITTEST(m_server->initSocket(12345), true);
    UNITTEST(m_server->start(), true);
    m_clientClientSide = new TcpClient("127.0.0.1", 12345);
    usleep(10000);

    UNITTEST(m_server->getNumberOfSockets(), 1);

    if(m_server->getNumberOfSockets() == 1)
    {
        m_clientServerSide = static_cast<TcpClient*>(m_server->getSocket(0));
    }
}

/**
 * checkLittleDataTransfer
 */
void
TcpClient_TcpServer_Test::checkLittleDataTransfer()
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
TcpClient_TcpServer_Test::checkBigDataTransfer()
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
TcpClient_TcpServer_Test::cleanupTestCase()
{
    UNITTEST(m_clientServerSide->closeSocket(), true);
    m_clientServerSide->closeSocket();
    UNITTEST(m_server->closeServer(), true);

    delete m_server;
    delete m_buffer;
}

}
}
