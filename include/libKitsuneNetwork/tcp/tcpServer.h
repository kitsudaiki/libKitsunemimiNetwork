#ifndef TCPSERVER_H
#define TCPSERVER_H

/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

/* Headerfiles für UNIX/Linux */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <commonThread.h>

namespace Kitsune
{
namespace Network
{
class TcpClient;

class TcpServer : public Kitsune::CommonThread
{
public:
    TcpServer();
    ~TcpServer();

    bool initSocket(const uint16_t port);
    TcpClient* waitForIncomingConnection();
    bool sendMessage(const std::string &message, int clientSocket);
    bool closeServer();

    uint32_t getNumberOfSockets();
    TcpClient* getSocket(const uint32_t pos);

protected:
    void run();

private:
    int m_serverSocket = 0;
    std::vector<TcpClient*> m_sockets;
    struct sockaddr_in m_server;
};

}
}

#endif // TCPSERVER_H
