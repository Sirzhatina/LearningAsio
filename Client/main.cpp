#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <Common/Common.hpp>
#include <string>

int main(int argc, char* argv[])
{
    constexpr std::uint16_t currentPort = 64;
    if (argc != 3)
    {
        std::cerr << "Using: <program> <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    using namespace boost::asio;

    io_context io;

    ip::tcp::socket sock(io, { ip::tcp::v4(), currentPort });
    ip::tcp::endpoint remote{ ip::address::from_string(argv[1]), ip::port_type(std::stoul(argv[2])) };

    std::string msg;
    Header msgSize;

    auto bodyHandler = [&msgSize](const boost::system::error_code& ec, std::size_t bytes)
    {
        if (!ec)
        {
            std::cout << "Body has been read successfully.\n";
        }
        else
        {
            std::cout << "Failed reading body.\n" << ec.value() << ": " << ec.message();
        }
        if (bytes != msgSize)
        {
            std::cout << "\nmsgSize and read bytes differ";
        }
    };

    auto headerHandler = [&sock, &msg, &msgSize, &bodyHandler](const boost::system::error_code& ec, std::size_t bytes)
    {
        if (!ec)
        {
            std::cout << "Header has been read successfully." << std::endl;
            msg.resize(msgSize);

            async_read(sock, buffer(msg, msgSize), bodyHandler);
        }
        else
        {
            std::cout << "Failed reading header.\n" << ec.value()  << ": " << ec.message();
        }
        if (headerSize != bytes)
        {
            std::cout << "\nExpected and read sizes differ";
        }
    };

    auto connectionHandler = [&sock, &msg, &msgSize, &headerHandler](const boost::system::error_code& ec)
    {
        if (!ec)
        {
            async_read(sock, buffer(&msgSize, headerSize), headerHandler);
        }
        else
        {
            std::cout << "Failed connection.\n" << ec.value() << ": " << ec.message();
        }
    };

    sock.async_connect(remote, connectionHandler);

    io.run();
    std::cout << msg;

    return 0;
}