/**
 *  @file    tcp_socket_tcp_server_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "tcp_socket_tcp_server_test.h"
#include <libKitsunemimiCommon/buffer/ring_buffer.h>

#include <libKitsunemimiNetwork/tcp/tcp_server.h>
#include <libKitsunemimiNetwork/tcp/tcp_socket.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * processMessageTcp-callback
 */
uint64_t processMessageTcp(void* target,
                           Kitsunemimi::RingBuffer* recvBuffer,
                           AbstractSocket*)
{
    DataBuffer* targetBuffer = static_cast<DataBuffer*>(target);
    const uint8_t* dataPointer = getDataPointer_RingBuffer(*recvBuffer, recvBuffer->usedSize);

    if(dataPointer == nullptr) {
        return 0;
    }

    addData_DataBuffer(*targetBuffer, dataPointer, recvBuffer->usedSize);
    return recvBuffer->usedSize;
}

/**
 * processConnectionTcp-callback
 */
void processConnectionTcp(void* target,
                          AbstractSocket* socket)
{
    socket->setMessageCallback(target, &processMessageTcp);
    socket->startThread();
}


TcpSocket_TcpServer_Test::TcpSocket_TcpServer_Test() :
    Kitsunemimi::CompareTestHelper("TcpSocket_TcpServer_Test")
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
TcpSocket_TcpServer_Test::initTestCase()
{
    m_buffer = new DataBuffer(1000);
    m_server = new TcpServer(m_buffer, &processConnectionTcp);
}

/**
 * checkConnectionInit
 */
void
TcpSocket_TcpServer_Test::checkConnectionInit()
{
    // init server
    TEST_EQUAL(m_server->getType(), AbstractServer::TCP_SERVER);
    TEST_EQUAL(m_server->initServer(12345), true);
    TEST_EQUAL(m_server->startThread(), true);

    // init client
    m_socketClientSide = new TcpSocket("127.0.0.1", 12345);
    TEST_EQUAL(m_socketClientSide->initClientSide(), true);
    TEST_EQUAL(m_socketClientSide->initClientSide(), true);
    TEST_EQUAL(m_socketClientSide->getType(), AbstractSocket::TCP_SOCKET);

    usleep(100000);

    TEST_EQUAL(m_server->getNumberOfSockets(), 1);

    if(m_server->getNumberOfSockets() == 1)
    {
        m_socketServerSide = static_cast<TcpSocket*>(m_server->getPendingSocket());
        TEST_EQUAL(m_socketServerSide->getType(), AbstractSocket::TCP_SOCKET);
        TEST_EQUAL(m_server->getNumberOfSockets(), 0);
    }
}

/**
 * checkLittleDataTransfer
 */
void
TcpSocket_TcpServer_Test::checkLittleDataTransfer()
{
    usleep(10000);

    std::string sendMessage("poipoipoi");
    TEST_EQUAL(m_socketClientSide->sendMessage(sendMessage), true);
    usleep(10000);
    TEST_EQUAL(m_buffer->bufferPosition, 9);

    if(m_buffer->bufferPosition == 9)
    {
        DataBuffer* buffer = m_buffer;
        uint64_t bufferSize = buffer->bufferPosition;
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
TcpSocket_TcpServer_Test::checkBigDataTransfer()
{
    std::string sendMessage = "poi";
    TEST_EQUAL(m_socketClientSide->sendMessage(sendMessage), true);
    for(uint32_t i = 0; i < 99999; i++)
    {
        m_socketClientSide->sendMessage(sendMessage);
    }

    usleep(10000);
    uint64_t totalIncom = m_buffer->bufferPosition;
    DataBuffer* dataBuffer = m_buffer;
    TEST_EQUAL(totalIncom, 300000);
    TEST_EQUAL(dataBuffer->bufferPosition, 300000);

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
TcpSocket_TcpServer_Test::cleanupTestCase()
{
    TEST_EQUAL(m_socketServerSide->closeSocket(), true);
    m_socketServerSide->closeSocket();
    TEST_EQUAL(m_server->closeServer(), true);

    delete m_server;
    delete m_buffer;
}

} // namespace Network
} // namespace Kitsunemimi
