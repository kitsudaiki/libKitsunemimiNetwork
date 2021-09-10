/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tcp/tcp_test.h>
#include <libKitsunemimiNetwork/unix/unix_domain_test.h>
#include <libKitsunemimiNetwork/tls_tcp/tls_tcp_test.h>

#include <libKitsunemimiPersistence/logger/logger.h>

using Kitsunemimi::Persistence::initConsoleLogger;

int main()
{
    //initConsoleLogger(true);

    Kitsunemimi::Network::UnixDomain_Test();
    Kitsunemimi::Network::Tcp_Test();
    Kitsunemimi::Network::TlsTcp_Test();
}
