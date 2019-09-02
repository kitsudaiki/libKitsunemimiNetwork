/**
 *  @file    tcp_client.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <tcp/tcp_client.h>
#include <iostream>
#include <network_trigger.h>
#include <cleanup_thread.h>

namespace Kitsune
{
namespace Network
{

/**
 * constructor for the client-side of the tcp-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 */
TcpClient::TcpClient(const std::string address,
                     const uint16_t port)
    : AbstractClient ()
{
    m_address = address;
    m_port = port;
    m_clientSide = true;
    initClientSide();
}

/**
 * constructor for the server-side of the tcp-connection, which is called by the
 * tcp-server for each incoming connection
 *
 * @param clientFd file-descriptor of the client-socket
 * @param client address for the client
 */
TcpClient::TcpClient(const int clientFd, sockaddr_in client)
    : AbstractClient ()
{
    m_clientSocket = clientFd;
    m_client = client;
    m_clientSide = false;
}

/**
 * destructor
 */
TcpClient::~TcpClient()
{
    closeSocket();
}

/**
 * init tcp-socket and connect to the server
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
TcpClient::initClientSide()
{
    struct sockaddr_in server;
    struct hostent* hostInfo;
    unsigned long addr;

    // create socket
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_clientSocket < 0) {
        return false;
    }

    // set TCP_NODELAY for sure
    int optval = 1;
    if(setsockopt(m_clientSocket, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int)) < 0) {
        printf("Cannot set TCP_NODELAY option on listen socket (%s)\n", strerror(errno));
    }

    // set server-address
    memset(&server, 0, sizeof(server));
    if((addr = inet_addr(m_address.c_str())) != INADDR_NONE) {
        memcpy((char*)&server.sin_addr, &addr, sizeof(addr));
    }
    else
    {
        // get server-connection via host-name instead of ip-address
        hostInfo = gethostbyname(m_address.c_str());
        if(hostInfo == nullptr) {
            return false;
        }
        memcpy((char*)&server.sin_addr, hostInfo->h_addr, hostInfo->h_length);
    }

    // set other informations
    server.sin_family = AF_INET;
    server.sin_port = htons(m_port);

    // create connection
    if(connect(m_clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        // TODO: correctly close socket
        m_clientSocket = 0;
        return false;
    }

    return true;
}

/**
 * wait for new incoming messages
 *
 * @return false, if receive failed or client is aborted, else true
 */
bool
TcpClient::waitForMessage()
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
    long recvSize = recv(m_clientSocket,
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
    for(uint64_t i = 0; i < m_trigger.size(); i++)
    {
        const uint64_t readBytes = m_trigger[i]->runTask(m_recvBuffer, this);

        m_recvBuffer.readPosition = (m_recvBuffer.readPosition + readBytes)
                                    % m_recvBuffer.totalBufferSize;
        m_recvBuffer.readWriteDiff -= readBytes;
    }

    return true;
}

/**
 * send a text-message over the socket
 *
 * @param message message to send
 *
 * @return false, if send failed or send was incomplete, else true
 */
bool
TcpClient::sendMessage(const std::string &message)
{
    const uint64_t messageLength = message.length();
    return sendMessage((uint8_t*)message.c_str(), messageLength);
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
TcpClient::sendMessage(const uint8_t* message,
                       const uint64_t numberOfBytes)
{
    // precheck if client is connected
    if(m_clientSocket == 0) {
        return false;
    }

    // send message
    const ssize_t successfulSended = send(m_clientSocket,
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
 * close the socket and schedule the deletion of the thread
 *
 * @return false, if already closed, else true
 */
bool
TcpClient::closeSocket()
{
    if(m_abort == true) {
        return false;
    }

    m_abort = true;

    // close socket if connected
    if(m_clientSocket >= 0)
    {
        shutdown(m_clientSocket, SHUT_RDWR);
        close(m_clientSocket);
        m_clientSocket = 0;
    }

    // add socket-thread to the cleanup-thread, because if the client triggers the close by itself,
    // when the other side of the connection triggers the close-process,
    // the thread would try to close itself, which would result into a deadlock.
    // That is the reason, why another thread sould process the delete of the client-thread.
    AbstractClient::m_cleanup->addClientForCleanup(this);

    return true;
}

} // namespace Network
} // namespace Kitsune
