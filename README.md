# libKitsunemimiNetwork

![Gitlab pipeline status](https://img.shields.io/gitlab/pipeline/tobiasanker/libKitsunemimiNetwork?label=build%20and%20test&style=flat-square)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/tobiasanker/libKitsunemimiNetwork?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/tobiasanker/libKitsunemimiNetwork?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-14-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

## Description

This is a small library for network connections. It provides servers and clients for unix-domain-sockets, tcp-sockets and ssl encrypted tcp-sockets. 

## Common Information aboud my projects

Here some common information about my projects and my code-styling. It's not complete and a bit short. I will write a styling-guide for my projects, where I will write this a bit longer with more explanation, why I make it like this.

### About my kitsune libraries

1. All my libraries beginning with `libKitsunemimi`, because I needed a naming to identify my own libraries and I decided to use `Kitsunemimi` as name, because Kitsunemimi are moe. ;)

2. The namespace of the code within the libraries is identically to the name of the library. So for example all content of `libKitsunemimiNetwork` has the namespace `Kitsunemimi::Network`.

3. If you only want to use the library, beside the binary you only ne the public methods and variables in the header-files, which are located in the `include`-directory of each `libKitsunemimi`-repo. I try my best to make these as small and self-explaining, as possible. 

### About my repositories in general

1. I programming only on Linux. Sorry, but I have no time for supporting Windows and Mac.

2. All my projects are mirrored to gitlab, where I have still my gitlab-ci runner.

3. In each project-repository is an `build.sh`-script to make it easier for you to build the project by yourself. It download and link all required git-repositories in the correct version and link it all. This script is also used by the gitlab-ci-runner and so it is everytime up-to-date.

4. All my projects have unit-tests, which are executed by the ci-runner in each commit. Most of the functions are tested. Primary some negative-tests are missing. I try to avoid mocks in the tests as much as possible. Even tcp-socket-tests using the localhost for real data-transfer-tests.

5. For the naming of variables, methods and classes, I use camel case and for the names of files and directories, I use snake case.

6. About my comments:

    - Many comments in my code seems to be a bit useless, but I like to use them for optical separation. I also write them, when I think I finished the programming task and while writing the comments I recheck the code under the comment again and often I find improvements or bugs while this.

    - At first I planned to use doxygen comment-style for methods and files, but I think I will change this, because while writing the current state of the code, I don't like it to write big comments for each simple self-explaining method.

    - I don't like it to write much comments into header-files. More exactly I absolutly hate it, when I have to search the provided functions of a header between a massive amount of comments. Header-files are for me primary to have an overview of all provided functions, which I want to see as fast as possible. Comments of functions, parameters and return-values in my code are only written into the source-files. So when something is unclear for a specific method, then look into the source-file. If the comment there are not helpful for you, then please write me a mail or an issue to fix this. 

7. I try to avoid templates and macros as much as possible. I don't dislike them, but I only use them, when I don't have another solution. 

## Build

I write my projects with the Qt-creator, but without Qt itself. 

### Requirements

#### Official repositories

paket | version
--- | ---
g++ | 6.3.0
qt5-qmake | 5.7.1
libssl1.1 | 1.1.0k
libssl-dev | 1.1.0k
libboost-filesystem-dev | 1.62
libsqlite3-dev | 3.16.2

This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsunemimi-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsunemimiCommon | v0.10.1 |  https://github.com/tobiasanker/libKitsunemimiCommon.git
libKitsunemimiPersistence | v0.7.0 |  https://github.com/tobiasanker/libKitsunemimiPersistence.git

### build library

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.

Before running the build-script:

```bash
.
└── libKitsunemimiNetwork
    ├── build.sh
    └── ...
```

After running the build-script:

```bash
.
├── build
│   ├── libKitsunemimiCommon
│   │   └── ...
│   ├── libKitsunemimiPersistence
│   │   └── ...
│   └── libKitsunemimiNetwork
│       └── ...
│
├── libKitsunemimiCommon
│   └── ...
├── libKitsunemimiPersistence
│   └── ...
├── libKitsunemimiNetwork
│   ├── build.sh
│   └── ...
│
└── result
    ├── include
    │   ├── libKitsunemimiCommon
    │   │   └── ...
    │   ├── libKitsunemimiPersistence
    │   │	└── ...
    │   └── libKitsunemimiNetwork
    │       └── ...
    │
    ├── libKitsunemimiCommon.so.0 -> libKitsunemimiCommon.so.0.10.1
    ├── libKitsunemimiCommon.so.0.10 -> libKitsunemimiCommon.so.0.10.1
    ├── libKitsunemimiCommon.so.0.10.1
    │
    ├── libKitsunemimiPersistence.so.0 -> libKitsunemimiPersistence.so.0.7.0
    ├── libKitsunemimiPersistence.so.0.7 -> libKitsunemimiPersistence.so.0.7.0
    ├── libKitsunemimiPersistence.so.0.7.0
    │
    ├── libKitsunemimiNetwork.so.0 -> libKitsunemimiNetwork.so.0.5.3
    ├── libKitsunemimiNetwork.so.0.5 -> libKitsunemimiNetwork.so.0.5.3
    └── libKitsunemimiNetwork.so.0.5.3
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
#include <libKitsunemimiCommon/data_buffer.h>

TlsTcpServer* server = nullptr;
TlsTcpSocket* socketClientSide = nullptr;
TlsTcpSocket* socketServerSide = nullptr;

// callback for new incoming messages
uint64_t processMessageTlsTcp(void* target,
                              MessageRingBuffer* recvBuffer,
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
    socket->start();
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
server->start();

// create a client which works as client and connect to the server
socketClientSide = new TlsTcpSocket("127.0.0.1",   // <- server-address
                                    12345,         // <- server-port
                                    "/tmp/cert.pem",
                                    "/tmp/key.pem");
// if the client should only send and never receive messages,
//    it doesn't need the following two lines. These init the buffer
//    for incoming messages and starting the thread of the client-socket
socketClientSide->addNetworkTrigger(buffer, &processMessageTlsTcp);
socketClientSide->start();
//..

// get socket on server-side, which was spawned by the incoming connection
if(server->getNumberOfSockets() > 0)
{
    socketServerSide = static_cast<TlsTcpSocket*>(m_server->getSocket(0));
}

// send data
socketClientSide->sendMessage(std::string("any message"));
// instead of socketClientSide you can use socketServerSide the same way

```


## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
