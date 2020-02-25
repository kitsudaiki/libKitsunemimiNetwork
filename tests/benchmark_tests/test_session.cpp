#include "test_session.h"

#include <libKitsunemimiNetwork/tcp/tcp_server.h>
#include <libKitsunemimiNetwork/tcp/tcp_socket.h>

#include <libKitsunemimiNetwork/unix/unix_domain_server.h>
#include <libKitsunemimiNetwork/unix/unix_domain_socket.h>

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/data_buffer.h>
#include <libKitsunemimiCommon/common_items/table_item.h>

#include <libKitsunemimiPersistence/logger/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * processMessageTcp-callback
 */
uint64_t processMessageTcp(void* target,
                           MessageRingBuffer* recvBuffer,
                           AbstractSocket* socket)
{
    TestSession* testClass = static_cast<TestSession*>(target);

    if(recvBuffer->readWriteDiff == 0)
    {
        return 0;
    }

    //delete data;
    //std::cout<<"testClass->m_sizeCounter "<<testClass->m_sizeCounter<<std::endl;
    testClass->m_sizeCounter += recvBuffer->readWriteDiff;

    if(socket->isClientSide() == false)
    {
        if(testClass->m_sizeCounter == testClass->m_size*100)
        {
            testClass->m_end = std::chrono::system_clock::now();
            float duration = std::chrono::duration_cast<chronoMicroSec>(testClass->m_end - testClass->m_start).count();
            duration /= 1000000.0f;
            const float speed = ((static_cast<float>(testClass->m_size*100)
                                 / (1024.0f*1024.0f*1024.0f))
                                 / duration) * 8;

            Kitsunemimi::TableItem result;

            result.addColumn("key");
            result.addColumn("value");

            result.addRow(std::vector<std::string>{"duration", std::to_string(duration) + " seconds"});
            result.addRow(std::vector<std::string>{"speed", std::to_string(speed) + " Gbits/sec"});

            std::cout<<result.toString()<<std::endl;

            exit(0);
        }
    }


    return recvBuffer->readWriteDiff;
}

/**
 * processConnection-callback
 */
void processConnection(void* target,
                       AbstractSocket* socket)
{
    std::cout<<"processConnection"<<std::endl;

    if(socket->isClientSide() == false)
    {
        TestSession* testClass = static_cast<TestSession*>(target);
        testClass->m_serverSession = socket;
        testClass->m_serverSession->setMessageCallback(testClass, &processMessageTcp);
        testClass->m_serverSession->startThread();
    }
}

/**
 * @brief TestSession::TestSession
 * @param address
 * @param port
 */
TestSession::TestSession(const std::string &address,
                         const uint16_t port,
                         const std::string &type)
{
    //Kitsunemimi::Persistence::initLogger("/tmp/", "benchmark", true, true);
    m_size = 1024*1024*1024;
    m_dataBuffer = new uint8_t[1024*1024];

    if(type == "tcp") {
        m_isTcp = true;
    } else {
        m_isTcp = false;
    }

    if(port == 0)
    {
        m_isClient = true;

        if(m_isTcp)
        {
            TcpServer* tcpServer = new TcpServer(this, &processConnection);
            m_server = tcpServer;
            assert(tcpServer->initServer(1234));
            assert(m_server->startThread());
        }
        else
        {
            UnixDomainServer* udsServer = new UnixDomainServer(this, &processConnection);
            m_server = udsServer;
            assert(udsServer->initServer("/tmp/sock.uds"));
            assert(m_server->startThread());
        }

        usleep(100000);

        if(m_isTcp) {
            m_clientSession = new TcpSocket("127.0.0.1", 1234);
        } else {
            m_clientSession = new UnixDomainSocket(std::string("/tmp/sock.uds"));
        }

        assert(m_clientSession->initClientSide());
        m_clientSession->setMessageCallback(this, &processMessageTcp);
        assert(m_clientSession->startThread());
    }
}

/**
 * @brief TestSession::sendLoop
 */
void
TestSession::sendLoop()
{
    if(m_isClient)
    {
        usleep(1000000);

        m_start = std::chrono::system_clock::now();
        for(int j = 0; j < 100; j++)
        {
            for(int i = 0; i < 8*1024; i++)
            {
                assert(m_clientSession->sendMessage(m_dataBuffer, 128*1024));
            }
        }
    }

    while(true)
    {
        usleep(10000);
    }
}

} // namespace Network
} // namespace Kitsunemimi
