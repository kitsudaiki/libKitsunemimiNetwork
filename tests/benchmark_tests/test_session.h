/**
 *  @file       test_session.h
 *
 *  @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright  MIT License
 */

#ifndef TEST_SESSION_H
#define TEST_SESSION_H

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include <libKitsunemimiCommon/test_helper/speed_test_helper.h>

namespace Kitsunemimi
{
struct RingBuffer;
struct DataBuffer;
namespace Network
{
class TcpServer;
class TcpSocket;
class UnixDomainSocket;
class UnixDomainServer;
class AbstractSocket;
class AbstractServer;

typedef std::chrono::microseconds chronoMicroSec;
typedef std::chrono::nanoseconds chronoNanoSec;
typedef std::chrono::seconds chronoSec;
typedef std::chrono::high_resolution_clock::time_point chronoTimePoint;
typedef std::chrono::high_resolution_clock chronoClock;

class TestSession
        : public Kitsunemimi::SpeedTestHelper
{
public:
    TestSession(const std::string &address,
                const uint16_t port,
                const std::string &type);
    void runTest();
    double calculateSpeed(double duration);

    bool m_isClient = false;
    bool m_isTcp = false;

    uint64_t m_totalSize = 0;
    uint64_t m_sizeCounter = 0;
    uint8_t* m_dataBuffer = nullptr;

    AbstractServer* m_server = nullptr;
    AbstractSocket* m_clientSession = nullptr;
    AbstractSocket* m_serverSession = nullptr;

    TimerSlot m_timeSlot;
    std::mutex m_cvMutex;
    std::condition_variable m_cv;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TEST_SESSION_H
