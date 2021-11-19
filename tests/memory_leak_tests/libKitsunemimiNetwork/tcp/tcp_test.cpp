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
                           AbstractSocket*)
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
                          AbstractSocket* socket)
{
    Tcp_Test* targetTest = static_cast<Tcp_Test*>(target);
    targetTest->m_socketServerSide = static_cast<TcpSocket*>(socket);
    socket->setMessageCallback(target, &processMessageTcp);
    socket->startThread();
}


Tcp_Test::Tcp_Test()
    : Kitsunemimi::MemoryLeakTestHelpter("Tcp_Test")
{
    ErrorContainer* error = nullptr;

    // init for one-time-allocations
    m_server = new TcpServer(this, &processConnectionTcp, "Tcp_Test");
    m_server->scheduleThreadForDeletion();
    sleep(2);

    // create new test-server
    REINIT_TEST();
    m_buffer = new DataBuffer(1000);
    error = new ErrorContainer();
    m_server = new TcpServer(this, &processConnectionTcp, "Tcp_Test");
    m_server->initServer(12345, *error);
    m_server->startThread();

        // test client create and delete
        m_socketClientSide = new TcpSocket("127.0.0.1", 12345, "Tcp_Test_client");
        m_socketClientSide->initClientSide(*error);
        sleep(2);

            // send messages
            std::string sendMessage("poipoipoi");
            m_socketClientSide->sendMessage(sendMessage, *error);
            usleep(100000);

            std::string sendMessage2 = "poi";
            m_socketClientSide->sendMessage(sendMessage2, *error);
            for(uint32_t i = 0; i < 99999; i++) {
                m_socketClientSide->sendMessage(sendMessage2, *error);
            }

        m_socketServerSide->closeSocket();
        m_socketServerSide->scheduleThreadForDeletion();
        m_socketClientSide->closeSocket();
        m_socketClientSide->scheduleThreadForDeletion();
        sleep(2);

    // clear test-server
    m_server->closeServer();
    m_server->scheduleThreadForDeletion();
    sleep(2);
    delete m_buffer;
    delete error;
    CHECK_MEMORY();
}

} // namespace Network
} // namespace Kitsunemimi
