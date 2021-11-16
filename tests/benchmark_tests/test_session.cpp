/**
 *  @file       test_session.cpp
 *
 *  @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright  MIT License
 */

#include "test_session.h"

#include <libKitsunemimiNetwork/tcp/tcp_server.h>
#include <libKitsunemimiNetwork/tcp/tcp_socket.h>

#include <libKitsunemimiNetwork/unix/unix_domain_server.h>
#include <libKitsunemimiNetwork/unix/unix_domain_socket.h>

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/buffer/ring_buffer.h>
#include <libKitsunemimiCommon/common_items/table_item.h>

#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * processMessageTcp-callback
 */
uint64_t processMessageTcp(void* target,
                           Kitsunemimi::RingBuffer* recvBuffer,
                           AbstractSocket* socket)
{
    TestSession* testClass = static_cast<TestSession*>(target);

    // precheck
    if(recvBuffer->usedSize == 0) {
        return 0;
    }

    if(socket->isClientSide() == false)
    {
        // handle test-message
        testClass->m_sizeCounter += recvBuffer->usedSize;
        if(testClass->m_sizeCounter == testClass->m_totalSize)
        {
            testClass->m_sizeCounter = 0;
            testClass->m_cv.notify_all();
        }
    }


    return recvBuffer->usedSize;
}

/**
 * processConnection-callback
 */
void processConnection(void* target,
                       AbstractSocket* socket)
{
    if(socket->isClientSide() == false)
    {
        TestSession* testClass = static_cast<TestSession*>(target);
        testClass->m_serverSession = socket;
        testClass->m_serverSession->setMessageCallback(testClass, &processMessageTcp);
        testClass->m_serverSession->startThread();
    }
}

/**
 * @brief TestSession
 */
TestSession::TestSession(const std::string &,
                         const uint16_t port,
                         const std::string &type)
{
    Kitsunemimi::initConsoleLogger(true);
    ErrorContainer error;

    // initialize global values
    m_totalSize = 1024l*1024l*1024l*10l;
    m_dataBuffer = new uint8_t[1024*1024];
    if(type == "tcp") {
        m_isTcp = true;
    } else {
        m_isTcp = false;
    }

    // initialize time-slot
    m_timeSlot.unitName = "Gbits/s";

    if(port == 0)
    {
        m_isClient = true;

        // create server
        if(m_isTcp)
        {
            // initialize tcp-server
            m_timeSlot.name = "tcp-speed";
            TcpServer* tcpServer = new TcpServer(this,
                                                 &processConnection,
                                                 "test-server");
            m_server = tcpServer;
            assert(tcpServer->initServer(1234, error));
            assert(m_server->startThread());
        }
        else
        {
            // initialize unix-domain-server
            m_timeSlot.name = "uds-speed";
            UnixDomainServer* udsServer = new UnixDomainServer(this,
                                                               &processConnection,
                                                               "test-server");
            m_server = udsServer;
            assert(udsServer->initServer("/tmp/sock.uds", error));
            assert(m_server->startThread());
        }

        usleep(100000);

        // create client
        if(m_isTcp) {
            m_clientSession = new TcpSocket("127.0.0.1", 1234, "test-client");
        } else {
            m_clientSession = new UnixDomainSocket(std::string("/tmp/sock.uds"), "test-client");
        }

        // start client
        assert(m_clientSession->initClientSide(error));
        m_clientSession->setMessageCallback(this, &processMessageTcp);
        assert(m_clientSession->startThread());
    }
}

/**
 * @brief TestSession::sendLoop
 */
void
TestSession::runTest()
{
    std::unique_lock<std::mutex> lock(m_cvMutex);
    ErrorContainer error;

    if(m_isClient)
    {
        usleep(1000000);

        // complete run
        for(int j = 0; j < 10; j++)
        {
            std::cout<<"run loop"<<std::endl;
            // single run
            m_timeSlot.startTimer();
            for(int i = 0; i < 10*8*1024; i++)
            {
                assert(m_clientSession->sendMessage(m_dataBuffer, 128*1024, error));
            }
            m_cv.wait(lock);

            m_timeSlot.stopTimer();
            m_timeSlot.values.push_back(calculateSpeed(m_timeSlot.getDuration(MICRO_SECONDS)));
        }
    }

    // create output of the test-result
    addToResult(m_timeSlot);
    printResult();
}

/**
 * @brief calculate speed
 *
 * @param duration duration in micro-seconds
 * @return speed in GBit/s
 */
double
TestSession::calculateSpeed(double duration)
{
    duration /= 1000000.0;
    const double speed = ((static_cast<double>(m_totalSize)
                          / (1024.0*1024.0*1024.0))
                          / duration) * 8.0;
    return speed;
}

} // namespace Network
} // namespace Kitsunemimi
