#ifndef TEST_SESSION_H
#define TEST_SESSION_H

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <chrono>

namespace Kitsunemimi
{
struct RingBuffer;
class DataBuffer;
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
{
public:
    TestSession(const std::string &address,
                const uint16_t port,
                const std::string &type);
    void sendLoop();

    bool m_isClient = false;
    bool m_isTcp = false;

    uint64_t m_size = 0;
    uint64_t m_sizeCounter = 0;
    uint8_t* m_dataBuffer = nullptr;

    AbstractServer* m_server = nullptr;
    AbstractSocket* m_clientSession = nullptr;
    AbstractSocket* m_serverSession = nullptr;

    std::chrono::high_resolution_clock::time_point m_start;
    std::chrono::high_resolution_clock::time_point m_end;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // TEST_SESSION_H
