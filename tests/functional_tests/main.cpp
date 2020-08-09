/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tcp/tcp_socket_tcp_server_test.h>
#include <libKitsunemimiNetwork/unix/unix_domain_socket_unix_domain_server_test.h>
#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_socket_tls_tcp_server_test.h>

#include <libKitsunemimiPersistence/logger/logger.h>

using Kitsunemimi::Persistence::initConsoleLogger;

int main()
{
    //initConsoleLogger(true);

    Kitsunemimi::Network::UnixDomainSocket_UnixDomainServer_Test();
    Kitsunemimi::Network::TcpSocket_TcpServer_Test();
    Kitsunemimi::Network::TlsTcpSocket_TlsTcpServer_Test();
}
