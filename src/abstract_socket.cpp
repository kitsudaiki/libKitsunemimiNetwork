/**
 *  @file    abstract_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/abstract_socket.h>
#include <libKitsunemimiCommon/buffer/ring_buffer.h>
#include <libKitsunemimiCommon/threading/cleanup_thread.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief AbstractSocket::AbstractSocket
 */
AbstractSocket::AbstractSocket(const std::string &threadName)
    : Kitsunemimi::Thread(threadName)
{
    m_recvBuffer = new Kitsunemimi::RingBuffer();
}

/**
 * @brief destructor, which close the socket before deletion
 */
AbstractSocket::~AbstractSocket()
{
    closeSocket();
}

/**
 * @brief get socket-type
 *
 * @return socket-type enum
 */
AbstractSocket::socketTypes
AbstractSocket::getType()
{
    return m_type;
}

/**
 * @brief check if socket is on client-side
 *
 * @return true, if socket is on client-side of the connection
 */
bool
AbstractSocket::isClientSide() const
{
    return m_isClientSide;
}

/**
 * @brief add new callback for incoming messages
 *
 * @param target
 * @param processMessage
 *
 * @return false, if object was nullptr, else true
 */
void
AbstractSocket::setMessageCallback(void* target,
                                   uint64_t (*processMessage)(void*,
                                                              Kitsunemimi::RingBuffer*,
                                                              AbstractSocket*))
{
    m_target = target;
    m_processMessage = processMessage;
}

/**
 * @brief send a text-message over the socket
 *
 * @param message message to send
 * @param error reference for error-output
 *
 * @return false, if send failed or send was incomplete, else true
 */
bool
AbstractSocket::sendMessage(const std::string &message, ErrorContainer &error)
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
bool
AbstractSocket::sendMessage(const void* message,
                            const uint64_t numberOfBytes,
                            ErrorContainer &error)
{
    // precheck if socket is connected
    if(m_socket == 0)
    {
        error.errorMessage = "socket is not connected";
        return false;
    }

    // send message
    while(m_lock.test_and_set(std::memory_order_acquire)) { asm(""); }

    const ssize_t successfulSended = sendData(m_socket,
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
 * @brief wait for new incoming messages
 *
 * @return false, if receive failed or socket is aborted, else true
 */
bool
AbstractSocket::waitForMessage()
{
    // precheck
    if(m_abort) {
        return true;
    }

    // calulate buffer-part for recv message
    const uint64_t writePosition = Kitsunemimi::getWritePosition_RingBuffer(*m_recvBuffer);
    const uint64_t spaceToEnd = Kitsunemimi::getSpaceToEnd_RingBuffer(*m_recvBuffer);

    // wait for incoming message
    const long recvSize = recvData(m_socket,
                                   &m_recvBuffer->data[writePosition],
                                   spaceToEnd,
                                   0);

    // handle error-cases
    if(recvSize <= 0
            || m_abort)
    {
        return true;
    }

    // increase the
    m_recvBuffer->usedSize = (m_recvBuffer->usedSize + static_cast<uint64_t>(recvSize));

    // process message via callback-function
    uint64_t readBytes = 0;
    do
    {
        readBytes = m_processMessage(m_target, m_recvBuffer, this);
        moveForward_RingBuffer(*m_recvBuffer, readBytes);
    }
    while(readBytes > 0);

    return true;
}

/**
 * @brief close the socket and schedule the deletion of the thread
 *
 * @return false, if already closed, else true
 */
bool
AbstractSocket::closeSocket()
{
    if(m_abort == true) {
        return false;
    }

    m_abort = true;

    // close socket if connected
    if(m_socket >= 0)
    {
        shutdown(m_socket, SHUT_RDWR);
        close(m_socket);
        m_socket = 0;
    }

    return true;
}

/**
 * @brief run-method for the thread-class
 */
void
AbstractSocket::run()
{
    while(m_abort == false) {
        waitForMessage();
    }
}

} // namespace Network
} // namespace Kitsunemimi
