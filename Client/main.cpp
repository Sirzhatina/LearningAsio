#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include "Client.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Using: <program> <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    using namespace boost::asio;

    Client cl( { ip::address::from_string(argv[1]), ip::port_type(std::stol(argv[2])) });

    cl.showMessage();

    return 0;
}