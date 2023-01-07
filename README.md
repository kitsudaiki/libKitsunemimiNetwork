# libKitsunemimiNetwork

![Github workfloat status](https://img.shields.io/github/actions/workflow/status/kitsudaiki/libKitsunemimiNetwork/build_test.yml?branch=develop&style=flat-square&label=build%20and%20test)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/kitsudaiki/libKitsunemimiNetwork?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/kitsudaiki/libKitsunemimiNetwork?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-17-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

## Description

This is a small library for network connections. It provides servers and clients for 

- unix-domain-sockets
- tcp-sockets
- tls encrypted tcp-sockets

## Build

### Requirements

name | repository | version | task
--- | --- | --- | ---
g++ | g++ | >= 8.0 | Compiler for the C++ code.
make | make | >= 4.0 | process the make-file, which is created by qmake to build the programm with g++
qmake | qt5-qmake | >= 5.0 | This package provides the tool qmake, which is similar to cmake and create the make-file for compilation.
ssl library | libssl-dev | >= 1.1 | encryption for tls connections

Installation on Ubuntu/Debian:

```bash
sudo apt-get install g++ make qt5-qmake libssl-dev
```

IMPORTANT: All my projects are only tested on Linux. 

### Kitsunemimi-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | v0.26.1 |  https://github.com/kitsudaiki/libKitsunemimiCommon.git

HINT: These Kitsunemimi-Libraries will be downloaded and build automatically with the build-script below.

### build library

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.


Run the following commands:

```
git clone https://github.com/kitsudaiki/libKitsunemimiNetwork.git
cd libKitsunemimiNetwork
./build.sh
cd ../result
```

It create automatic a `build` and `result` directory in the directory, where you have cloned the project. At first it build all into the `build`-directory and after all build-steps are finished, it copy the include directory from the cloned repository and the build library into the `result`-directory. So you have all in one single place.

Tested on Debian and Ubuntu. If you use Centos, Arch, etc and the build-script fails on your machine, then please write me a mail and I will try to fix the script.

## Usage

### Init-overview

The following snippets show only the differences in initializing the different server and clients. The rest (send messages, close connections and so on) is basically identical and is schown by the complete example after this overview.

#### Unix-domain-connection

- server:

```cpp
UnixDomainServer udsServer("/tmp/sock.uds");
TemplateServer<UnixDomainServer>* server = nullptr;
m_server = new TemplateServer<UnixDomainServer>(std::move(udsServer),
                                                this,
                                                &processConnectionUnixDomain,
                                               "UnixDomain_Test");      // <- base-name for threads of server and clients

server->initServer(error)
```

- client:

```cpp
UnixDomainSocket udsSocket("/tmp/sock.uds");        // <- file-path , whiere the unix-domain-server is listen
m_socketClientSide = new TemplateSocket<UnixDomainSocket>(std::move(udsSocket),
                                                          "UnixDomain_Test_client");   // <- thread-name for the client
TemplateSocket<UnixDomainSocket>* ssocketClientSide = nullptr;
socketClientSide->initConnection(error)
```

#### TCP-connection

- server:

```cpp
// create tcp-server
TcpServer tcpServer(12345);                                     // <- init server with port
TemplateServer<TcpServer>* server = nullptr;
server = new TemplateServer<TcpServer>(std::move(tcpServer),
                                       buffer,                    // <- demo-buffer, which is forwarded to the 
                                                                 //        target void-pointer in the callback
                                       &processConnectionTlsTcp,  // <- callback for new incoming connections
                                       "Tcp_Test");               // <- base-name for threads of server and clients

server->initServer(error)
```

- client:

```cpp
TcpSocket tcpSocket("127.0.0.1",                                      // <- server-address
                    12345);                                           // <- server-port
TemplateSocket<TcpSocket>* ssocketClientSide = nullptr;
ssocketClientSide = new TemplateSocket<TcpSocket>(std::move(tcpSocket), 
                                                  "Tcp_Test_client");      // <- thread-name for the client
socketClientSide->initConnection(error)
```


#### TLS-encrypted TCP-connection

- server:

```cpp
// create tcp-server
TcpServer tcpServer(12345);                                       // <- init server with port
TlsTcpServer tlsTcpServer(std::move(tcpServer),
                          "/tmp/cert.pem",                        // <- path to certificate-file for tls-encryption
                          "/tmp/key.pem");                        // <- path to key-file for tls-encryption
TemplateServer<TlsTcpServer>* server = nullptr;
server = new TemplateServer<TlsTcpServer>(std::move(tlsTcpServer),
                                          buffer,                    // <- demo-buffer, which is forwarded to the 
                                                                     //        target void-pointer in the callback
                                          &processConnectionTlsTcp,  // <- callback for new incoming connections
                                          "TlsTcp_Test");            // <- base-name for threads of server and clients

server->initServer(error)
```

- client:

```cpp
TcpSocket tcpSocket("127.0.0.1",    // <- server-address
                    12345);         // <- server-port
TlsTcpSocket tlsTcpSocket(std::move(tcpSocket),
                          "/tmp/cert.pem",             // <- path to certificate-file for tls-encryption
                          "/tmp/key.pem");             // <- path to key-file for tls-encryption
TemplateSocket<TlsTcpSocket>* ssocketClientSide = nullptr;
ssocketClientSide = new TemplateSocket<TlsTcpSocket>(std::move(tlsTcpSocket), 
                                                     "TlsTcp_Test_client");      // <- thread-name for the client
socketClientSide->initConnection(error)
```


### Complete example

Example to create server and client with TCP-connectiona and TLS-encryption:

```cpp
#include <libKitsunemimiNetwork/netserver.h>
#include <libKitsunemimiNetwork/net_socket.h>
#include <libKitsunemimiCommon/buffer/data_buffer.h>

using namespace Kitsunemimi::Network;

// callback for new incoming messages
uint64_t processMessageTlsTcp(void* target,
                              Kitsunemimi::RingBuffer* recvBuffer,
                              AbstractSocket*)
{
    // here in this example the demo-buffer, which was registered in the server
    // is converted back from the void-pointer into the original object-pointer
    Kitsunemimi::DataBuffer* targetBuffer = static_cast<Kitsunemimi::DataBuffer*>(target);

    // get data from the message-ring-buffer
    const uint8_t* dataPointer = getDataPointer(*recvBuffer, numberOfBytesToRead);
    // this checks, if numberOfBytesToRead is available in the buffer and if that
    // is the case, it returns a pointer the the beginning of the buffer, else it
    // returns a nullptr

    // do what you want

    // return the number of byte, you have processed from the ring-buffer
    return numberOfProcessedBytes;
}

// callback for new incoming connections
void processConnection(void* target,
                       AbstractSocket* socket)
{
    // set callback-method for incoming messages on the new socket
    // you can also create a new buffer here and don't need to forward the void-pointer
    socket->setMessageCallback(target, &processMessageTlsTcp);

    // start the thread of the socket
    socket->startThread();
}

// init the demo-buffer from above
Kitsunemimi::DataBuffer* buffer = new Kitsunemimi::DataBuffer(1000);
Kitsunemimi::ErrorContainer error;

TemplateServer<TlsTcpServer>* server = nullptr;
TemplateSocket<TlsTcpSocket>* socketClientSide = nullptr;


//================================================================================
//                                    SERVER
//================================================================================

// create tcp-server
TcpServer tcpServer(12345);                                       // <- init server with port
TlsTcpServer tlsTcpServer(std::move(tcpServer),
                          "/tmp/cert.pem",                        // <- path to certificate-file for tls-encryption
                          "/tmp/key.pem");                        // <- path to key-file for tls-encryption
server = new TemplateServer<TlsTcpServer>(std::move(tlsTcpServer),
                                          buffer,                    // <- demo-buffer, which is forwarded to the 
                                                                     //        target void-pointer in the callback
                                          &processConnectionTlsTcp,  // <- callback for new incoming connections
                                          "TlsTcp_Test");            // <- base-name for threads of server and clients

// start listening on the port
if(server->initServer(error) == false) 
{
    // do error-handling
    LOG_ERROR(error);
}
                                    
// start the thread, so it can create a socket for every incoming 
//    connection in the background
server->startThread();


//================================================================================
//                                    CLIENT
//================================================================================

TcpSocket tcpSocket("127.0.0.1",    // <- server-address
                    12345);         // <- server-port
TlsTcpSocket tlsTcpSocket(std::move(tcpSocket),
                          "/tmp/cert.pem",
                          "/tmp/key.pem");
socketClientSide = new TemplateSocket<TlsTcpSocket>(std::move(tlsTcpSocket), 
                                                    "TlsTcp_Test_client");       // <- thread-name for the client
if(socketClientSide->initConnection(error) == false) 
{
    // do error-handling
    LOG_ERROR(error);
}

// if the client should only send and never receive messages,
//    it doesn't need the following two lines. These init the buffer
//    for incoming messages and starting the thread of the client-socket
socketClientSide->setMessageCallback(buffer, &processMessageTlsTcp);
socketClientSide->startThread();

// send data
socketClientSide->sendMessage("any message", error);
// instead of socketClientSide you can use socketServerSide the same way


//================================================================================
//                                    CLOSE_ALL
//================================================================================

// teminate client connection
socketClientSide->closeSocket();
socketClientSide->scheduleThreadForDeletion();

// teminate server
server->closeServer();
server->scheduleThreadForDeletion();
```

## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
