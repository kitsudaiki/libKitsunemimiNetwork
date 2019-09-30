/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsuneNetwork/tcp/tcp_socket_tcp_server_test.h>
#include <libKitsuneNetwork/unix/unix_domain_socket_unix_domain_server_test.h>
#include <libKitsuneNetwork/tls_tcp/tls_tcp_socket_tls_tcp_server_test.h>

#include <logger/logger.h>

using Kitsune::Persistence::initLogger;

int main()
{
    //initLogger("/tmp", "testlog", true, true);

    Kitsune::Network::TcpSocket_TcpServer_Test();
    Kitsune::Network::UnixDomainSocket_UnixDomainServer_Test();
    Kitsune::Network::TlsTcpSocket_TlsTcpServer_Test();
}
