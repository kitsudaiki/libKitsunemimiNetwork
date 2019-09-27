/**
 *  @file    tls_tcp_socket_tls_tcp_server_test.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "tls_tcp_socket_tls_tcp_server_test.h"
#include <buffering/data_buffer.h>

#include <tls_tcp/tls_tcp_server.h>
#include <tls_tcp/tls_tcp_socket.h>
#include <dummy_buffer.h>
#include <income_trigger.h>
#include <cert_init.h>

namespace Kitsune
{
namespace Network
{

TlsTcpSocket_TcpServer_Test::TlsTcpSocket_TcpServer_Test() :
    Kitsune::Common::UnitTest("TlsTcpSocket_TcpServer_Test")
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
TlsTcpSocket_TcpServer_Test::initTestCase()
{
    writeTestCerts();

    m_buffer = new DummyBuffer();
    m_incomeTrigger = new IncomeTrigger();
    m_server = new TlsTcpServer(std::string("/tmp/cert.pem"),
                                std::string("/tmp/key.pem"),
                                m_buffer,
                                m_incomeTrigger);
}

/**
 * checkConnectionInit
 */
void
TlsTcpSocket_TcpServer_Test::checkConnectionInit()
{
    // init server
    UNITTEST(m_server->getType(), AbstractServer::TLS_TCP_SERVER);
    UNITTEST(m_server->initServer(12345), true);
    UNITTEST(m_server->start(), true);

    // init client
    m_socketClientSide = new TlsTcpSocket("127.0.0.1",
                                          12345,
                                          "/tmp/cert.pem",
                                          "/tmp/key.pem");
    UNITTEST(m_socketClientSide->initClientSide(), true);
    UNITTEST(m_socketClientSide->getType(), AbstractSocket::TLS_TCP_SOCKET);

    usleep(10000);

    UNITTEST(m_server->getNumberOfSockets(), 1);

    if(m_server->getNumberOfSockets() == 1)
    {
        m_socketServerSide = static_cast<TlsTcpSocket*>(m_server->getSocket(0));
        UNITTEST(m_socketServerSide->getType(), AbstractSocket::TLS_TCP_SOCKET);
    }
}

/**
 * checkLittleDataTransfer
 */
void
TlsTcpSocket_TcpServer_Test::checkLittleDataTransfer()
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
TlsTcpSocket_TcpServer_Test::checkBigDataTransfer()
{
    std::string sendMessage = "poi";
    UNITTEST(m_socketClientSide->sendMessage(sendMessage), true);
    for(uint32_t i = 0; i < 99999; i++)
    {
        m_socketClientSide->sendMessage(sendMessage);
    }
    usleep(100000);
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
TlsTcpSocket_TcpServer_Test::cleanupTestCase()
{
    UNITTEST(m_socketServerSide->closeSocket(), true);
    m_socketServerSide->closeSocket();
    UNITTEST(m_server->closeServer(), true);

    delete m_server;
    delete m_buffer;
}

} // namespace Network
} // namespace Kitsune
