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
#include <commonDataBuffer.h>

#include <tcp/tcpServer.h>
#include <tcp/tcpClient.h>

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
    m_server = new TcpServer();
}

void TcpTest::checkConnectionInit()
{
    UNITTEST(m_server->initSocket(12345), true);
    UNITTEST(m_server->start(), true);
    m_clientClientSide = new TcpClient("127.0.0.1", 12345);
    sleep(1);
    UNITTEST(m_server->getNumberOfSockets(), 1);
    if(m_server->getNumberOfSockets() == 1) {
        m_clientServerSide = m_server->getSocket(0);
        m_clientServerSide->start();
    }
}

void TcpTest::checkLittleDataTransfer()
{
    std::string sendMessage("poipoipoi");
    UNITTEST(m_clientClientSide->sendMessage(sendMessage), true);
    sleep(2);
    UNITTEST(m_clientServerSide->getNumberOfBuffer(), 1);
    if(m_clientServerSide->getNumberOfBuffer() == 1)
    {
        CommonDataBuffer* buffer = m_clientServerSide->getCommonDataBuffer(0);
        uint64_t bufferSize = buffer->getNumberOfWrittenBytes();
        char recvMessage[bufferSize];
        memcpy(recvMessage, buffer->getBufferPointer(), bufferSize);
        UNITTEST(bufferSize, 9);
        UNITTEST(recvMessage[2], sendMessage.at(2));
        UNITTEST(m_clientServerSide->clearBuffer(), true);
        UNITTEST(m_clientServerSide->clearBuffer(), false);
    }
}

void TcpTest::checkBigDataTransfer()
{
    std::string sendMessage;
    for(uint32_t i = 0; i < 100000; i++) {
        sendMessage.append("poi");
    }
    UNITTEST(m_clientClientSide->sendMessage(sendMessage), true);
    sleep(1);
    uint32_t bufferSize = m_clientServerSide->getNumberOfBuffer();
    uint32_t totalIncom = 0;
    for(uint32_t i = 0; i < bufferSize; i++) {
        totalIncom += m_clientServerSide->getCommonDataBuffer(i)->getNumberOfWrittenBytes();
    }
    UNITTEST(totalIncom, sendMessage.size());
}

void TcpTest::cleanupTestCase()
{
    m_clientServerSide->stop();
    m_server->stop();
    UNITTEST(m_clientServerSide->closeSocket(), true);
    UNITTEST(m_server->closeServer(), true);
    m_clientClientSide->closeSocket();

    delete m_clientServerSide;
    delete m_clientClientSide;
    delete m_server;
}

}
}
