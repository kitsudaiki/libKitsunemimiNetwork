/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsuneNetwork/tcp/tcp_client_tcp_server_test.h>
#include <libKitsuneNetwork/unix/unix_client_unix_server_test.h>
#include <libKitsuneNetwork/tls_tcp/tls_tcp_client_tls_tcp_server_test.h>

int main()
{
    Kitsune::Network::TcpClient_TcpServer_Test();
    Kitsune::Network::UnixClient_UnixServer_Test();
    Kitsune::Network::TlsTcpClient_TcpServer_Test();
}
