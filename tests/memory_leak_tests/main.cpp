/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tcp/tcp_test.h>
#include <libKitsunemimiNetwork/unix/unix_domain_test.h>

#include <libKitsunemimiCommon/logger.h>

int main()
{
    Kitsunemimi::Network::UnixDomain_Test();
    Kitsunemimi::Network::Tcp_Test();
}
