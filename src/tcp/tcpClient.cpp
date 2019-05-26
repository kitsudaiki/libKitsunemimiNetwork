/**
 *  @file    tcpClient.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include <tcp/tcpClient.h>
#include <networkTrigger.h>
#include <iostream>

namespace Kitsune
{
namespace Network
{

Kitsune::Network::CleanupThread* TcpClient::m_cleanup = nullptr;

/**
 * @brief TcpClient::TcpClient
 */
TcpClient::TcpClient(const std::string address,
                     const uint16_t port)
{
    if(m_cleanup == nullptr)
    {
        m_cleanup = new Kitsune::Network::CleanupThread();
        m_cleanup->start();
    }
    m_address = address;
    m_port = port;
    m_clientSide = true;
    initClientSide();
}

/**
 * @brief TcpClient::TcpClient
 * @param clientFd
 * @param client
 */
TcpClient::TcpClient(const int clientFd, sockaddr_in client)
{
    if(m_cleanup == nullptr)
    {
        m_cleanup = new Kitsune::Network::CleanupThread();
        m_cleanup->start();
    }
    m_clientSocket = clientFd;
    m_client = client;
    m_clientSide = false;
}

/**
 * @brief TcpClient::~TcpClient
 */
TcpClient::~TcpClient()
{
    if(m_clientSocket >= 0)
    {
        shutdown(m_clientSocket, SHUT_RDWR);
        close(m_clientSocket);
        m_clientSocket = 0;
    }
}

/**
 * @brief TcpClient::addNetworkTrigger
 * @param trigger
 * @return
 */
bool
TcpClient::addNetworkTrigger(NetworkTrigger *trigger)
{
    if(trigger == nullptr) {
        return false;
    }
    m_trigger.push_back(trigger);
    return true;
}

/**
 * @brief TcpClient::removeNetworkTrigger
 * @param index
 * @return
 */
bool
TcpClient::removeNetworkTrigger(const uint32_t index)
{
    if(m_trigger.size() <= index) {
        return false;
    }
    m_trigger.erase(m_trigger.begin() + index);
    return true;
}

/**
 * @brief TcpClient::clearNetworkTrigger
 */
void
TcpClient::clearNetworkTrigger()
{
    m_trigger.clear();
}

/**
 * @brief TcpClient::init
 * @return
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
        m_clientSocket = 0;
        return false;
    }

    return true;
}

/**
 * @brief TcpClient::waitForMessage
 * @return
 */
bool
TcpClient::waitForMessage()
{
    // calulate buffer-part for recv message
    uint32_t writePosition = (m_recvBuffer.readPosition + m_recvBuffer.readWriteDiff) % m_recvBuffer.totalBufferSize;
    if (static_cast<uint32_t>(m_recvBuffer.totalBufferSize) == writePosition) {
        writePosition = 0;
    }

    uint32_t spaceToEnd = 0;
    if(writePosition < m_recvBuffer.readPosition) {
        spaceToEnd = m_recvBuffer.readPosition - writePosition;
    }
    else {
        spaceToEnd = static_cast<uint32_t>(m_recvBuffer.totalBufferSize) - writePosition;
    }

    // wait for incoming message
    long recvSize = recv(m_clientSocket,
                         &m_recvBuffer.data[writePosition],
                         spaceToEnd,
                         0);

    if(recvSize < 0 || m_abort) {
        return false;
    }
    m_recvBuffer.readWriteDiff = (m_recvBuffer.readWriteDiff + static_cast<uint32_t>(recvSize));

    for(uint32_t i = 0; i < m_trigger.size(); i++)
    {
        uint32_t readBytes = m_trigger[i]->runTask(m_recvBuffer, this);

        m_recvBuffer.readPosition = (m_recvBuffer.readPosition + readBytes) % m_recvBuffer.totalBufferSize;
        m_recvBuffer.readWriteDiff -= readBytes;
    }

    return true;
}

/**
 * @brief TcpClient::sendMessage
 * @param message
 * @return
 */
bool
TcpClient::sendMessage(const std::string &message)
{
    const uint32_t messageLength = message.length();
    return sendMessage((uint8_t*)message.c_str(), messageLength);
}

/**
 * @brief TcpClient::sendMessage
 * @param message
 * @param numberOfBytes
 * @return
 */
bool
TcpClient::sendMessage(uint8_t* message, const uint32_t numberOfBytes)
{
    if(m_clientSocket == 0) {
        return false;
    }
    const ssize_t successfulSended = send(m_clientSocket,
                                          message,
                                          numberOfBytes,
                                          MSG_NOSIGNAL);
    if(successfulSended < -1 || successfulSended != numberOfBytes) {
        return false;
    }
    return true;
}

/**
 * @brief TcpClient::closeSocket
 * @return
 */
bool
TcpClient::closeSocket()
{
    if(m_abort == true) {
        return false;
    }

    m_abort = true;

    if(m_clientSocket >= 0)
    {
        shutdown(m_clientSocket, SHUT_RDWR);
        close(m_clientSocket);
        m_clientSocket = 0;
    }

    TcpClient::m_cleanup->addClientForCleanup(this);

    return true;
}

/**
 * @brief TcpClient::run
 */
void
TcpClient::run()
{
    while(!m_abort) {
        waitForMessage();
    }
}

}
}
