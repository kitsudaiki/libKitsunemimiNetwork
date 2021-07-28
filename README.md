# libKitsunemimiNetwork

![Gitlab pipeline status](https://img.shields.io/gitlab/pipeline/kitsudaiki/libKitsunemimiNetwork?label=build%20and%20test&style=flat-square)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/kitsudaiki/libKitsunemimiNetwork?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/kitsudaiki/libKitsunemimiNetwork?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-14-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

## Description

This is a small library for network connections. It provides servers and clients for unix-domain-sockets, tcp-sockets and ssl encrypted tcp-sockets. 

## Build

### Requirements

name | repository | version | task
--- | --- | --- | ---
g++ | g++ | >= 6.0 | Compiler for the C++ code.
make | make | >= 4.0 | process the make-file, which is created by qmake to build the programm with g++
qmake | qt5-qmake | >= 5.0 | This package provides the tool qmake, which is similar to cmake and create the make-file for compilation.
boost-filesystem library | libboost-filesystem-dev | >= 1.6 | interactions with files and directories on the system
ssl library | libssl-dev | >= 1.1 | encryption for tls connections

Installation on Ubuntu/Debian:

```bash
sudo apt-get install g++ make qt5-qmake libboost-filesystem-dev libssl-dev
```

IMPORTANT: All my projects are only tested on Linux. 

### Kitsunemimi-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | v0.18.0 |  https://github.com/kitsudaiki/libKitsunemimiCommon.git
libKitsunemimiPersistence | v0.10.2 | https://github.com/kitsudaiki/libKitsunemimiPersistence.git

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

(sorry, that the usage-chapter here is very short, but while writing I found some points, which must become better.)

This libray contains servers and clients for unix-domain-sockets, tcp-sockets and ssl encrypted tcp-sockets. Each socket and server runs in its own thread.

The inheritance tree of the classes has the following look. For servers its the same with only servers instead of sockets.

```
        Thread (libKitsunemimiCommon)
          ^
          |
    AbstractSocket
    ^            ^
    |            |
TcpSocket    UnixDomainSocket
    ^
    |
TlsTcpSocket
```


Example to create server and socket:
(the `TlsTcpSocket` of the example can also replaced by the `UnixDomainSocket` or `TcpSocket`. Only the values of the conectructor are different)

```cpp
#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_server.h>
#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_socket.h>
#include <libKitsunemimiCommon/buffer/data_buffer.h>

TlsTcpServer* server = nullptr;
TlsTcpSocket* socketClientSide = nullptr;
TlsTcpSocket* socketServerSide = nullptr;

// callback for new incoming messages
uint64_t processMessageTlsTcp(void* target,
                              RingBuffer* recvBuffer,
                              AbstractSocket*)
{
	// here in this example the demo-buffer, which was registered in the server
	// is converted back from the void-pointer into the original object-pointer
    Common::DataBuffer* targetBuffer = static_cast<Common::DataBuffer*>(target);

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
void processConnectionTlsTcp(void* target,
                             AbstractSocket* socket)
{
	// set callback-method for incoming messages on the new socket
	// you can also create a new buffer here and don't need to forward the void-pointer
    socket->setMessageCallback(target, &processMessageTlsTcp);

    // start the thread of the socket
    socket->startThread();
}

// init the demo-buffer from above
Common::DataBuffer* buffer = new Common::DataBuffer(1000);

// create server
server = new TlsTcpServer(buffer,                      // <- demo-buffer, which is forwarded to the 
                                                       //        target void-pointer in the callback
                          &processConnectionTlsTcp,
                          "/tmp/cert.pem",
                          "/tmp/key.pem");   // <- callback-method for new incoming connections
                                    
// let the server listen on port 12345
server->initServer(12345);
// start the thread, so it can create a socket for every incoming 
//    connection in the background
server->startThread();

// create a client which works as client and connect to the server
socketClientSide = new TlsTcpSocket("127.0.0.1",   // <- server-address
                                    12345,         // <- server-port
                                    "/tmp/cert.pem",
                                    "/tmp/key.pem");
// if the client should only send and never receive messages,
//    it doesn't need the following two lines. These init the buffer
//    for incoming messages and starting the thread of the client-socket
socketClientSide->addNetworkTrigger(buffer, &processMessageTlsTcp);
socketClientSide->startThread();
//..

// get socket on server-side, which was spawned by the incoming connection
if(server->getNumberOfSockets() > 0)
{
    socketServerSide = static_cast<TlsTcpSocket*>(m_server->getSocket(0));
}

// send data
socketClientSide->sendMessage(std::string("any message"));
// instead of socketClientSide you can use socketServerSide the same way

// teminate connectioin
socketClientSide->closeSocket();
socketClientSide->scheduleThreadForDeletion();
```


## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
