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

namespace Kitsune
{
namespace Network
{

/**
 * @brief TcpClient::TcpClient
 */
TcpClient::TcpClient(const std::string address,
                     const uint16_t port)
{
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
    m_fd = clientFd;
    m_client = client;
    m_clientSide = false;
}

/**
 * @brief TcpClient::~TcpClient
 */
TcpClient::~TcpClient()
{
    closeSocket();
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
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_fd < 0) {
        return false;
    }

    // set TCP_NODELAY for sure
    //int optval = 1;
    //if(setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int)) < 0) {
    //    printf("Cannot set TCP_NODELAY option on listen socket (%s)\n", strerror(errno));
    //}

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
    if(connect(m_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
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
    long recvSize = recv(m_fd,
                         m_recvBuffer,
                         RCVBUFSIZE,
                         0);

    if(recvSize < 0) {
        return false;
    }

    for(uint32_t i = 0; i < m_trigger.size(); i++)
    {
        m_trigger[i]->runTask(m_recvBuffer, recvSize, nullptr);
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
    const uint32_t successfulSended = send(m_fd, message, numberOfBytes, 0);
    if(successfulSended != numberOfBytes) {
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
    stop();
    if(m_fd >= 0)
    {
        close(m_fd);
        m_fd = 0;
        return true;
    }
    return false;
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
