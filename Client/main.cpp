#include <boost/asio.hpp>
#include <iostream>
#include <string>

constexpr std::uint16_t currentPort = 64;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Using: <program> <address> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    using namespace boost::asio;

    io_context io;

    ip::tcp::socket sock(io, { ip::tcp::v4(), currentPort });
    ip::tcp::endpoint remote{ ip::address::from_string(argv[1]), ip::port_type(std::stoul(argv[2])) };

    std::uint16_t data;
    std::size_t size = sizeof(std::uint16_t);

    sock.async_connect(remote, [&sock, &data, &size](const boost::system::error_code& ec)
    {
        if (!ec)
        {
            boost::system::error_code ec;
            std::size_t bytesRead = read(sock, buffer(&data, size), ec);

            if (!ec)
            {
                std::cout << "Data has been read successfully: " << data;
            }
            else
            {
                std::cout << "Failed reading data";
            }
            if (size != bytesRead)
            {
                std::cout << "\nExpected and read sizes differ";
            }
        }
        else
        {
            std::cout << "Failed connection";
        }
    });

    io.run();
    return 0;
}