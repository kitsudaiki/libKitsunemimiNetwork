/**
 *  @file       main.cpp
 *
 *  @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright  MIT License
 */

#include <libKitsunemimiCommon/logger.h>
#include <libKitsunemimiArgs/arg_parser.h>
#include <test_session.h>

int main(int argc, char *argv[])
{
    //initConsoleLogger(true);
    Kitsunemimi::Args::ArgParser argParser;

    argParser.registerString("address,a",
                             "address to connect (Default: 127.0.0.1)");
    argParser.registerInteger("port,p",
                              "port where to listen (Default: 4321)");
    argParser.registerString("socket,s",
                             "type: tcp or uds (Default: tcp)");
    argParser.registerString("transfer-type,t",
                             "type of transfer: stream, standalone or request (Default: stream)");
    argParser.registerInteger("package-size",
                              "Test-package-size in byte(Default: 128 KiB)",
                              true,
                              true);

    bool ret = argParser.parse(argc, argv);
    if(ret == false) {
        return 1;
    }

    uint16_t port = 4321;
    std::string address = "127.0.0.1";
    std::string socket = "tcp";
    std::string transferType = "stream";

    if(argParser.wasSet("address")) {
        address = argParser.getStringValues("address").at(0);
    }
    if(argParser.wasSet("port")) {
        port = static_cast<uint16_t>(argParser.getIntValues("port").at(0));
    }
    if(argParser.wasSet("socket")) {
        socket = argParser.getStringValues("socket").at(0);
    }
    if(argParser.wasSet("transfer-type")) {
        transferType = argParser.getStringValues("transfer-type").at(0);
    }

    // debug-output
    std::cout<<"address: "<<address<<std::endl;
    std::cout<<"port: "<<static_cast<int>(port)<<std::endl;
    std::cout<<"type: "<<socket<<std::endl;

    // check type
    if(socket != "tcp"
            && socket != "uds")
    {
        std::cout<<"ERROR: type \""<<socket<<"\" is unknown. Choose \"tcp\" or \"uds\"."<<std::endl;
        exit(1);
    }

    // run test
    Kitsunemimi::Network::TestSession testSession(address, port, socket);
    testSession.runTest();
}
