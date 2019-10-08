/**
 *  @file    tcp_socket_tcp_server_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "tcp_socket_tcp_server_test.h"
#include <libKitsuneCommon/data_buffer.h>

#include <libKitsuneNetwork/tcp/tcp_server.h>
#include <libKitsuneNetwork/tcp/tcp_socket.h>

namespace Kitsune
{
namespace Network
{

/**
 * processMessageTcp-callback
 */
uint64_t processMessageTcp(void* target,
                           MessageRingBuffer* recvBuffer,
                           AbstractSocket*)
{
    Common::DataBuffer* targetBuffer = static_cast<Common::DataBuffer*>(target);
    const uint8_t* dataPointer = getDataPointer(*recvBuffer, recvBuffer->readWriteDiff);

    if(dataPointer == nullptr) {
        return 0;
    }

    addDataToBuffer(targetBuffer, dataPointer, recvBuffer->readWriteDiff);
    return recvBuffer->readWriteDiff;
}

/**
 * processConnectionTcp-callback
 */
void processConnectionTcp(void* target,
                          AbstractSocket* socket)
{
    socket->setMessageCallback(target, &processMessageTcp);
    socket->start();
}


TcpSocket_TcpServer_Test::TcpSocket_TcpServer_Test() :
    Kitsune::Common::UnitTest("TcpSocket_TcpServer_Test")
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
    m_buffer = new Common::DataBuffer(1000);
    m_server = new TcpServer(m_buffer, &processConnectionTcp);
}

/**
 * checkConnectionInit
 */
void
TcpSocket_TcpServer_Test::checkConnectionInit()
{
    // init server
    UNITTEST(m_server->getType(), AbstractServer::TCP_SERVER);
    UNITTEST(m_server->initServer(12345), true);
    UNITTEST(m_server->start(), true);

    // init client
    m_socketClientSide = new TcpSocket("127.0.0.1", 12345);
    UNITTEST(m_socketClientSide->initClientSide(), true);
    UNITTEST(m_socketClientSide->initClientSide(), true);
    UNITTEST(m_socketClientSide->getType(), AbstractSocket::TCP_SOCKET);

    usleep(10000);

    UNITTEST(m_server->getNumberOfSockets(), 1);

    if(m_server->getNumberOfSockets() == 1)
    {
        m_socketServerSide = static_cast<TcpSocket*>(m_server->getSocket(0));
        UNITTEST(m_socketServerSide->getType(), AbstractSocket::TCP_SOCKET);
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
    UNITTEST(m_socketClientSide->sendMessage(sendMessage), true);
    usleep(10000);
    UNITTEST(m_buffer->bufferPosition, 9);


    if(m_buffer->bufferPosition == 9)
    {
        uint64_t bufferSize = m_buffer->bufferPosition;
        char recvMessage[bufferSize];
        memcpy(recvMessage, m_buffer->data, bufferSize);
        UNITTEST(bufferSize, 9);
        UNITTEST(recvMessage[2], sendMessage.at(2));
        resetBuffer(m_buffer, 1000);
    }
}

/**
 * checkBigDataTransfer
 */
void
TcpSocket_TcpServer_Test::checkBigDataTransfer()
{
    std::string sendMessage = "poi";
    UNITTEST(m_socketClientSide->sendMessage(sendMessage), true);
    for(uint32_t i = 0; i < 99999; i++)
    {
        m_socketClientSide->sendMessage(sendMessage);
    }
    usleep(10000);
    uint64_t totalIncom = m_buffer->bufferPosition;
    Common::DataBuffer* dataBuffer = m_buffer;
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
TcpSocket_TcpServer_Test::cleanupTestCase()
{
    UNITTEST(m_socketServerSide->closeSocket(), true);
    m_socketServerSide->closeSocket();
    UNITTEST(m_server->closeServer(), true);

    delete m_server;
    delete m_buffer;
}

} // namespace Network
} // namespace Kitsune
