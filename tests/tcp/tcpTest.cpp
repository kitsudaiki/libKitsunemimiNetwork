/**
 *  @file    tcpTest.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "tcpTest.h"
#include <iostream>
#include <buffering/commonDataBuffer.h>

#include <tcp/tcpServer.h>
#include <tcp/tcpClient.h>
#include <testBuffer.h>

namespace Kitsune
{
namespace Network
{

TcpTest::TcpTest() : Kitsune::CommonTest("TcpTest")
{
    initTestCase();
    checkConnectionInit();
    checkLittleDataTransfer();
    checkBigDataTransfer();
    cleanupTestCase();
}

void TcpTest::initTestCase()
{
    m_buffer = new TestBuffer();
    m_server = new TcpServer(m_buffer);
}

void TcpTest::checkConnectionInit()
{
    UNITTEST(m_server->initSocket(12345), true);
    UNITTEST(m_server->start(), true);
    m_clientClientSide = new TcpClient("127.0.0.1", 12345);
    usleep(10000);

    UNITTEST(m_server->getNumberOfSockets(), 1);

    if(m_server->getNumberOfSockets() == 1)
    {
        m_clientServerSide = m_server->getSocket(0);
    }
}

void TcpTest::checkLittleDataTransfer()
{
    usleep(10000);

    std::string sendMessage("poipoipoi");
    UNITTEST(m_clientClientSide->sendMessage(sendMessage), true);
    usleep(10000);
    UNITTEST(m_buffer->getNumberOfWrittenBytes(), 9);


    if(m_buffer->getNumberOfWrittenBytes() == 9)
    {
        CommonDataBuffer* buffer = m_buffer->getBuffer();
        uint64_t bufferSize = buffer->bufferPosition;
        char recvMessage[bufferSize];
        memcpy(recvMessage, buffer->data, bufferSize);
        UNITTEST(bufferSize, 9);
        UNITTEST(recvMessage[2], sendMessage.at(2));
        m_buffer->clearBuffer();
    }
}

void TcpTest::checkBigDataTransfer()
{
    std::string sendMessage;
    for(uint32_t i = 0; i < 100000; i++) {
        sendMessage.append("poi");
    }
    UNITTEST(m_clientClientSide->sendMessage(sendMessage), true);
    usleep(10000);
    uint64_t totalIncom = m_buffer->getNumberOfWrittenBytes();
    UNITTEST(totalIncom, sendMessage.size());
}

void TcpTest::cleanupTestCase()
{
    UNITTEST(m_clientServerSide->closeSocket(), true);
    m_clientServerSide->closeSocket();
    UNITTEST(m_server->closeServer(), true);

    delete m_server;
    delete m_buffer;
}

}
}
