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

    boost::system::error_code ec;
    sock.connect(remote, ec);
    if (ec)
    {
        std::cout << "Failed connection";
        return EXIT_FAILURE;
    }

    std::uint16_t data;
    std::size_t size = sizeof(std::uint16_t);

    std::size_t bytesRead = read(sock, buffer(&data, size), ec);

    if (ec)
    {
        std::cout << "Failed reading data";
        return EXIT_FAILURE;
    }
    if (size != bytesRead)
    {
        std::cout << "Expected and read sizes differ";
        return EXIT_FAILURE;
    }
    std::cout << "Data has been read successfully: " << data;

    io.run();
    return 0;
}