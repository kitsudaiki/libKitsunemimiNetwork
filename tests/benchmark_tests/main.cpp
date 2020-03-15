/**
 *  @file       main.cpp
 *
 *  @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright  MIT License
 */

#include <libKitsunemimiPersistence/logger/logger.h>
#include <boost/program_options.hpp>
#include <test_session.h>

using Kitsunemimi::Persistence::initLogger;
namespace argParser = boost::program_options;


int main(int argc, char *argv[])
{
    // define arg-parser
    argParser::options_description desc("Allowed options");
    desc.add_options()
        (
            "help,h", // -h and --help for help-text
            "produce help message"
        )
        (
            "address,a",
            argParser::value<std::string>(),
            "address to connect"
        )
        (
            "port,p",
            argParser::value<uint16_t>(),
            "port where to listen"
        )
        (
            "type,t",
            argParser::value<std::string>(),
            "type: tcp or uds"
        )
    ;

    // exec arg-parser
    argParser::variables_map vm;
    argParser::store(argParser::parse_command_line(argc, argv, desc), vm);
    argParser::notify(vm);

    // help-arg
    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    // init values
    uint16_t port = 0;
    std::string address = "";
    std::string type = "";

    // get values from command line
    if(vm.count("address")) {
        address = vm["address"].as<std::string>();
    }
    if(vm.count("port")) {
        port = vm["port"].as<uint16_t>();
    }
    if(vm.count("type")) {
        type = vm["type"].as<std::string>();
    }

    // debug-output
    std::cout<<"address: "<<address<<std::endl;
    std::cout<<"port: "<<(int)port<<std::endl;
    std::cout<<"type: "<<type<<std::endl;

    // check type
    if(type != "tcp"
            && type != "uds")
    {
        std::cout<<"ERROR: type \""<<type<<"\" is unknown. Choose \"tcp\" or \"uds\"."<<std::endl;;
        exit(1);
    }

    // run test
    Kitsunemimi::Network::TestSession testSession(address, port, type);
    testSession.runTest();
}
