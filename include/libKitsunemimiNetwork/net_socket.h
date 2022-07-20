/**
 *  @file    net_socket.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cinttypes>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <errno.h>
#include <atomic>

#include <libKitsunemimiCommon/threading/thread.h>
#include <libKitsunemimiCommon/logger.h>
#include <libKitsunemimiCommon/buffer/ring_buffer.h>
#include <libKitsunemimiCommon/threading/cleanup_thread.h>

namespace Kitsunemimi
{
struct RingBuffer;
namespace Network
{

class TcpSocket;
class TlsTcpSocket;
class UnixDomainSocket;

template<class T>
class NetSocket
        : public Kitsunemimi::Thread
{
public:
    NetSocket(T&& socket,
              const std::string &threadName)
        : Kitsunemimi::Thread(threadName)
    {
        m_socket = std::move(socket);
    }

    ~NetSocket()
    {
        closeSocket();
    }

    /**
     * @brief add new callback for incoming messages
     *
     * @param target
     * @param processMessage
     *
     * @return false, if object was nullptr, else true
     */
    void setMessageCallback(void *target,
                            uint64_t (*processMessage)(void*,
                                                       Kitsunemimi::RingBuffer*,
                                                       NetSocket<TcpSocket>*))
    {
        m_target = target;
        m_processMessage = processMessage;
    }

    void setMessageCallback(void *target,
                            uint64_t (*processMessage)(void*,
                                                       Kitsunemimi::RingBuffer*,
                                                       NetSocket<TlsTcpSocket>*))
    {
        m_target = target;
        m_processMessage = processMessage;
    }


    void setMessageCallback(void *target,
                            uint64_t (*processMessage)(void*,
                                                       Kitsunemimi::RingBuffer*,
                                                       NetSocket<UnixDomainSocket>*))
    {
        m_target = target;
        m_processMessage = processMessage;
    }

    /**
     * @brief get socket-type
     *
     * @return socket-type enum
     */
    uint32_t getType()
    {
        return m_socket.m_type;
    }

    /**
     * @brief check if socket is on client-side
     *
     * @return true, if socket is on client-side of the connection
     */
    bool isClientSide() const
    {
        return m_socket.m_isClientSide;
    }

    /**
     * @brief send a text-message over the socket
     *
     * @param message message to send
     * @param error reference for error-output
     *
     * @return false, if send failed or send was incomplete, else true
     */
    bool sendMessage(const std::string &message, ErrorContainer &error)
    {
        const uint64_t messageLength = message.length();
        return sendMessage(static_cast<const void*>(message.c_str()), messageLength, error);
    }

    /**
     * @brief send a byte-buffer over the tcp-socket
     *
     * @param message byte-buffer to send
     * @param numberOfBytes number of bytes to send
     * @param error reference for error-output
     *
     * @return false, if send failed or send was incomplete, else true
     */
    bool sendMessage(const void* message,
                     const uint64_t numberOfBytes,
                     ErrorContainer &error)
    {
        // precheck if socket is connected
        if(m_socket.m_socketFd == 0)
        {
            error.addMeesage("socket is not connected");
            return false;
        }

        // send message
        while(m_lock.test_and_set(std::memory_order_acquire)) { asm(""); }

        const ssize_t successfulSended = m_socket.sendData(m_socket.m_socketFd,
                                                           message,
                                                           numberOfBytes,
                                                           MSG_NOSIGNAL);
        m_lock.clear(std::memory_order_release);

        // check if the message was completely send
        if(successfulSended < -1
                || successfulSended != static_cast<long>(numberOfBytes))
        {
            return false;
        }

        return true;
    }

    /**
     * @brief close the socket and schedule the deletion of the thread
     *
     * @return false, if already closed, else true
     */
    bool closeSocket()
    {
        if(m_abort == true) {
            return false;
        }

        m_abort = true;

        // close socket if connected
        if(m_socket.m_socketFd >= 0)
        {
            shutdown(m_socket.m_socketFd, SHUT_RDWR);
            close(m_socket.m_socketFd);
            m_socket.m_socketFd = 0;
        }

        // make sure, that the thread is out of the function recvData before further
        // deleteing the thread (maximum wait-time = 10ms)
        int32_t timeout = 100;
        while(m_isfullyClosed == false
              && timeout > 0)
        {
            usleep(100);
            timeout--;
        }

        return true;
    }

private:
    Kitsunemimi::RingBuffer m_recvBuffer;
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
    T m_socket;

    // callback-parameter
    void* m_target = nullptr;
    uint64_t (*m_processMessage)(void*, RingBuffer*, NetSocket*);

    /**
     * @brief run-method for the thread-class
     */
    void run()
    {
        while(m_abort == false) {
            waitForMessage();
        }
        m_isfullyClosed = true;
    }

    /**
     * @brief wait for new incoming messages
     *
     * @return false, if receive failed or socket is aborted, else true
     */
    bool waitForMessage()
    {
        // precheck
        if(m_abort) {
            return true;
        }

        // calulate buffer-part for recv message
        const uint64_t writePosition = Kitsunemimi::getWritePosition_RingBuffer(m_recvBuffer);
        const uint64_t spaceToEnd = Kitsunemimi::getSpaceToEnd_RingBuffer(m_recvBuffer);

        // wait for incoming message
        const long recvSize = m_socket.recvData(m_socket.m_socketFd,
                                                &m_recvBuffer.data[writePosition],
                                                spaceToEnd,
                                                0);

        // handle error-cases
        if(recvSize <= 0
                || m_abort)
        {
            return true;
        }

        // increase the
        m_recvBuffer.usedSize = (m_recvBuffer.usedSize + static_cast<uint64_t>(recvSize));

        // process message via callback-function
        uint64_t readBytes = 0;
        do
        {
            readBytes = m_processMessage(m_target, &m_recvBuffer, this);
            moveForward_RingBuffer(m_recvBuffer, readBytes);
        }
        while(readBytes > 0);

        return true;
    }

private:
    bool m_isfullyClosed = false;
};

} // namespace Network
} // namespace Kitsunemimi

#endif // NET_SOCKET_H
