/**
 *  @file    tcpClient.h
 *  @author  Tobias Anker
 *
 *  @section DESCRIPTION
 *
 *  TODO: Description
 */

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <string>
#include <commonThread.h>

namespace Kitsune
{
class CommonDataBuffer;

namespace Network
{

#define RCVBUFSIZE 8192

class TcpClient : public Kitsune::CommonThread
{
public:
    TcpClient(const std::string address,
              const uint16_t port);
    TcpClient(const int clientFd,
              struct sockaddr_in client);
    ~TcpClient();

    void setNewBufferSize(uint32_t numberOfBlocks);

    bool sendMessage(const std::string &message);
    bool sendMessage(uint8_t* message, const uint32_t numberOfBytes);

    bool closeSocket();

protected:
    void run();

private:
    bool m_clientSide = false;
    std::string m_address = "";
    uint16_t m_port = 0;

    Kitsune::CommonDataBuffer* m_incomingBuffer = nullptr;
    uint32_t m_bufferSize = 1;  // number of blocks

    int m_fd = 0;
    sockaddr_in m_client;

    bool initClientSide();
    bool waitForMessage();
};

}
}

#endif // TCPCLIENT_H
