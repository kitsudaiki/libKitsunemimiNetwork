/**
 *  @file    abstract_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "abstract_socket.h"

#include <cleanup_thread.h>
#include <message_trigger.h>

namespace Kitsune
{
namespace Network
{

Kitsune::Network::CleanupThread* AbstractSocket::m_cleanup = nullptr;

/**
 * @brief AbstractSocket::AbstractSocket
 */
AbstractSocket::AbstractSocket()
{
    if(m_cleanup == nullptr)
    {
        m_cleanup = new Kitsune::Network::CleanupThread();
        m_cleanup->start();
    }
}

/**
 * @brief AbstractSocket::~AbstractSocket
 */
AbstractSocket::~AbstractSocket()
{
    closeSocket();
}

/**
 * @brief AbstractSocket::getType
 * @return
 */
AbstractSocket::socketTypes
AbstractSocket::getType()
{
    return m_type;
}

/**
 * add new trigger-object for incoming messages
 *
 * @param trigger new trigger-object
 *
 * @return false, if object was nullptr, else true
 */
void
AbstractSocket::setMessageTrigger(MessageTrigger *trigger)
{
    m_trigger = trigger;
}

/**
 * send a text-message over the socket
 *
 * @param message message to send
 *
 * @return false, if send failed or send was incomplete, else true
 */
bool
AbstractSocket::sendMessage(const std::string &message)
{
    const uint64_t messageLength = message.length();
    return sendMessage(static_cast<const void*>(message.c_str()), messageLength);
}

/**
 * send a byte-buffer over the tcp-socket
 *
 * @param message byte-buffer to send
 * @param numberOfBytes number of bytes to send
 *
 * @return false, if send failed or send was incomplete, else true
 */
bool
AbstractSocket::sendMessage(const void* message,
                            const uint64_t numberOfBytes)
{
    // precheck if socket is connected
    if(m_socket == 0) {
        return false;
    }

    // send message
    const ssize_t successfulSended = sendData(m_socket,
                                              message,
                                              numberOfBytes,
                                              MSG_NOSIGNAL);

    // check if the message was completely send
    if(successfulSended < -1
            || successfulSended != static_cast<long>(numberOfBytes))
    {
        return false;
    }

    return true;
}

/**
 * wait for new incoming messages
 *
 * @return false, if receive failed or socket is aborted, else true
 */
bool
AbstractSocket::waitForMessage()
{
    // precheck
    if(m_abort) {
        return false;
    }

    // calulate buffer-part for recv message
    uint64_t writePosition = (m_recvBuffer.readPosition + m_recvBuffer.readWriteDiff)
                             % m_recvBuffer.totalBufferSize;
    if(m_recvBuffer.totalBufferSize == writePosition) {
        writePosition = 0;
    }

    uint64_t spaceToEnd = m_recvBuffer.totalBufferSize - writePosition;
    if(writePosition < m_recvBuffer.readPosition) {
        spaceToEnd = m_recvBuffer.readPosition - writePosition;
    }

    // wait for incoming message
    long recvSize = recvData(m_socket,
                             &m_recvBuffer.data[writePosition],
                             spaceToEnd,
                             0);

    // handle error-cases
    if(recvSize <= 0
            || m_abort)
    {
        return false;
    }

    // increase the
    m_recvBuffer.readWriteDiff = (m_recvBuffer.readWriteDiff + static_cast<uint64_t>(recvSize));

    // add all trigger to the new socket
    if(m_trigger != nullptr)
    {
        const uint64_t readBytes = m_trigger->runTask(m_recvBuffer, this);

        m_recvBuffer.readPosition = (m_recvBuffer.readPosition + readBytes)
                                    % m_recvBuffer.totalBufferSize;
        m_recvBuffer.readWriteDiff -= readBytes;
    }

    return true;
}

/**
 * close the socket and schedule the deletion of the thread
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

    // add socket-thread to the cleanup-thread, because if the socket triggers the close by itself,
    // when the other side of the connection triggers the close-process,
    // the thread would try to close itself, which would result into a deadlock.
    // That is the reason, why another thread sould process the delete of the socket-thread.
    AbstractSocket::m_cleanup->addSocketForCleanup(this);

    return true;
}

/**
 * run-method for the thread-class
 */
void
AbstractSocket::run()
{
    while(!m_abort)
    {
        waitForMessage();
    }
}

} // namespace Network
} // namespace Kitsune
