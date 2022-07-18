/**
 *  @file    tcp_socket_tcp_server_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "tcp_test.h"
#include <libKitsunemimiCommon/buffer/ring_buffer.h>

#include <libKitsunemimiNetwork/tcp/tcp_server.h>
#include <libKitsunemimiNetwork/tcp/tcp_socket.h>
#include <libKitsunemimiNetwork/net_socket.h>
#include <libKitsunemimiNetwork/net_server.h>
#include <libKitsunemimiCommon/threading/thread_handler.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * processMessageTcp-callback
 */
uint64_t processMessageTcp(void* target,
                           Kitsunemimi::RingBuffer* recvBuffer,
                           NetSocket<TcpSocket>*)
{
    Tcp_Test* targetTest = static_cast<Tcp_Test*>(target);
    const uint8_t* dataPointer = getDataPointer_RingBuffer(*recvBuffer, recvBuffer->usedSize);
    if(dataPointer == nullptr) {
        return 0;
    }

    addData_DataBuffer(*targetTest->m_buffer, dataPointer, recvBuffer->usedSize);
    return recvBuffer->usedSize;
}

/**
 * processConnectionTcp-callback
 */
void processConnectionTcp(void* target,
                          NetSocket<TcpSocket>* socket)
{
    Tcp_Test* targetTest = static_cast<Tcp_Test*>(target);
    targetTest->m_socketServerSide = socket;
    socket->setMessageCallback(target, &processMessageTcp);
    socket->startThread();
}


Tcp_Test::Tcp_Test() :
    Kitsunemimi::CompareTestHelper("Tcp_Test")
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
Tcp_Test::initTestCase()
{
    m_buffer = new DataBuffer(1000);
}

/**
 * checkConnectionInit
 */
void
Tcp_Test::checkConnectionInit()
{
    ErrorContainer error;

    // init server
    TcpServer tcpServer(12345);
    TEST_EQUAL(tcpServer.initServer(error), true);
    m_server = new NetServer<TcpServer>(std::move(tcpServer),
                                        this,
                                        &processConnectionTcp,
                                        "Tcp_Test");

    TEST_EQUAL(m_server->getType(), 2);
    TEST_EQUAL(m_server->startThread(), true);

    // init client
    TcpSocket tcpSocket("127.0.0.1", 12345);
    TEST_EQUAL(tcpSocket.initClientSide(error), true);
    TEST_EQUAL(tcpSocket.initClientSide(error), true);
    m_socketClientSide = new NetSocket<TcpSocket>(std::move(tcpSocket),
                                                  "Tcp_Test_client");
    TEST_EQUAL(m_socketClientSide->getType(), 2);

    usleep(100000);
}

/**
 * checkLittleDataTransfer
 */
void
Tcp_Test::checkLittleDataTransfer()
{
    usleep(10000);
    ErrorContainer error;

    std::string sendMessage("poipoipoi");
    TEST_EQUAL(m_socketClientSide->sendMessage(sendMessage, error), true);
    usleep(10000);
    TEST_EQUAL(m_buffer->usedBufferSize, 9);

    if(m_buffer->usedBufferSize == 9)
    {
        DataBuffer* buffer = m_buffer;
        uint64_t bufferSize = buffer->usedBufferSize;
        char recvMessage[bufferSize];
        memcpy(recvMessage, buffer->data, bufferSize);
        TEST_EQUAL(bufferSize, 9);
        TEST_EQUAL(recvMessage[2], sendMessage.at(2));
        reset_DataBuffer(*m_buffer, 1000);
    }
}

/**
 * checkBigDataTransfer
 */
void
Tcp_Test::checkBigDataTransfer()
{
    ErrorContainer error;

    std::string sendMessage = "poi";
    TEST_EQUAL(m_socketClientSide->sendMessage(sendMessage, error), true);
    for(uint32_t i = 0; i < 99999; i++) {
        m_socketClientSide->sendMessage(sendMessage, error);
    }

    usleep(100000);
    uint64_t totalIncom = m_buffer->usedBufferSize;
    DataBuffer* dataBuffer = m_buffer;
    TEST_EQUAL(totalIncom, 300000);
    TEST_EQUAL(dataBuffer->usedBufferSize, 300000);

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

    TEST_EQUAL(numberOfPois, 100000);
}

/**
 * cleanupTestCase
 */
void
Tcp_Test::cleanupTestCase()
{
    TEST_EQUAL(m_socketServerSide->closeSocket(), true);
    TEST_EQUAL(m_server->closeServer(), true);
    TEST_EQUAL(m_socketServerSide->scheduleThreadForDeletion(), true);
    TEST_EQUAL(m_server->scheduleThreadForDeletion(), true);

    delete m_buffer;
}

} // namespace Network
} // namespace Kitsunemimi
