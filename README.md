# libKitsuneNetwork

## Description

This is a small library for network connections. It provides servers and clients for unix-sockets, tcp-sockets and ssl encrypted tcp-sockets. 


### About my kitsune libraries

My first libraries hab really common names like libCommon and so on, but I thought this could be hard to find my code between other libraries, when my projects will become bigger. So I decided to make a common name over all my libraries, similar to the boost-libraries, for a better handling. I decided to use `Kitsune` as name for my projects, because Kitsunemimi are moe. ;)

### About my repositories in general

At the moment there are over 10 Kitsune-libraries an multiple tools in my private repos. I plan to make all open-source, but I have much to learn about open-source-projects. Most of all I have to improve my ticket-handling. So please help me to improve my workflow.

All my projects are mirrored to gitlab, where I have still my gitlab-ci runner. 

I don't really like it to write much comments into header-files. More exactly I absolutly hate it, when I have to search the provided functions of a header between a massive amount of comments. Header-files are for me primary to have an overview, of all provided functions, which I have see as fast as possible. Comments of functions, parameters and return-values in my code are only written into the source-file. So when something is unclear for a specific method, than look into the source-file. If the comment there are not helpful for you, then please write me a mail or an issue to fix this. 

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

#### Kitsune-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsuneCommon | 0.5.1 |  https://github.com/tobiasanker/libKitsuneCommon.git
libKitsunePersistence | 0.3.0 |  https://github.com/tobiasanker/libKitsunePersistence.git

### build library

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.

Before running the build-script:

```bash
.
└── libKitsuneNetwork
    ├── build.sh
    └── ...
```

After running the build-script:

```bash
.
├── build
│   ├── libKitsuneCommon
│   │   └── ...
│   ├── libKitsunePersistence
│   │   └── ...
│   └── libKitsuneNetwork
│       └── ...
│
├── libKitsuneCommon
│   └── ...
├── libKitsunePersistence
│   └── ...
├── libKitsuneNetwork
│   ├── build.sh
│   └── ...
│
└── result
    ├── include
    │   ├── libKitsuneCommon
    │   │   └── ...
    │   ├── libKitsunePersistence
    │   │	└── ...
    │   └── libKitsuneNetwork
    │       └── ...
    │
    ├── libKitsuneCommon.so -> libKitsuneCommon.so.0.5.1
    ├── libKitsuneCommon.so.0 -> libKitsuneCommon.so.0.5.1
    ├── libKitsuneCommon.so.0.5 -> libKitsuneCommon.so.0.5.1
    ├── libKitsuneCommon.so.0.5.1
    │
    ├── libKitsunePersistence.so -> libKitsunePersistence.so.0.3.0
    ├── libKitsunePersistence.so.0 -> libKitsunePersistence.so.0.3.0
    ├── libKitsunePersistence.so.0.3 -> libKitsunePersistence.so.0.3.0
    ├── libKitsunePersistence.so.0.3.0
    │
    ├── libKitsuneNetwork.so -> libKitsuneNetwork.so.0.2.0
    ├── libKitsuneNetwork.so.0 -> libKitsuneNetwork.so.0.2.0
    ├── libKitsuneNetwork.so.0.2 -> libKitsuneNetwork.so.0.2.0
    └── libKitsuneNetwork.so.0.2.0
```

It create automatic a `build` and `result` directory in the directory, where you have cloned the project. At first it build all into the `build`-directory and after all build-steps are finished, it copy the include directory from the cloned repository and the build library into the `result`-directory. So you have all in one single place.

Tested on Debian and Ubuntu. If you use Centos, Arch, etc and the build-script fails on your machine, then please write me a mail and I will try to fix the script.


## Usage

(sorry, that the usage-chapter here is very short, but while writing I found some points, which must become better.)

This libray contains servers and clients for unix-sockets, tcp-sockets and ssl encrypted tcp-sockets. Each socket and server runs in its own thread.

The inheritance tree of the classes has the following look. For servers its the same with only servers instead of sockets.

```
        Thread (libKitsuneCommon)
          ^
          |
    AbstractSocket
	^            ^
    |            |
TcpSocket    UnixSocket
    ^
    |
TlsTcpSocket
```

Because each socket run in its own thread, this thread have to handle incoming messages. So the class to handle these messages have to inherite the class `NetworkTrigger` (`network_trigger.h`). The `runTask` of this class is called whenever the socket get some data. This has to append to the server and sockets. See following examples:

```cpp
#include <network_trigger.h>
#include <abstract_socket.h>

class DemoBuffer : public NetworkTrigger
{
public:
    DemoBuffer() {};
    ~DemoBuffer() {};

	uint64_t
	DemoBuffer::runTask(MessageRingBuffer &recvBuffer,
	                    AbstractSocket* socket)
	{
		// example
		const uint8_t* dataPointer = getDataPointer(recvBuffer, NUMBER_OF_BYTES);
	    // do something with the dataPointer
	    // recvBuffer contains the data and socket is the socket, which had the message received
	}

};
```

Example to create server and socket:

```cpp
#include <tls_tcp/tls_tcp_server.h>
#include <tls_tcp/tls_tcp_socket.h>

TlsTcpServer* server = nullptr;
TlsTcpSocket* socketClientSide = nullptr;
TlsTcpSocket* socketServerSide = nullptr;

// init the demo-buffer from above
DemoBuffer* buffer = new DemoBuffer();

// create server on port 12345 and start his thread
server = new TlsTcpServer("/tmp/cert.pem",
                          "/tmp/key.pem",
                          buffer);  // <- demo-buffer, which is given to any
                                    //    socket which is spawned by the server
server->initServer(12345);
server->start();

// create a client which works as client and connect to the server
socketClientSide = new TlsTcpSocket("127.0.0.1",
                                    12345,
                                    "/tmp/cert.pem",
                                    "/tmp/key.pem");
socketClientSide->addNetworkTrigger(buffer);

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
