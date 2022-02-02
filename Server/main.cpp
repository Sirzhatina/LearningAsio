#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <chrono>


int main(int argc, char* argv[])
{
    constexpr std::uint16_t port = 25;
    
    std::cout << "Server started!" << std::endl;
    using namespace boost::asio;

    io_context io;

    ip::tcp::acceptor acc(io, ip::tcp::endpoint{ ip::tcp::v4(), port });

    using socket     = ip::tcp::socket;
    using error_code = boost::system::error_code;

    socket client(io);
    error_code ec;

    acc.accept(client, ec);

    if (ec)
    {
        std::cerr << "Failed connection.";
        return EXIT_FAILURE;
    }

    std::uint16_t data = 232;
    std::size_t size = sizeof(std::uint16_t);

    std::size_t bytesWritten = write(client, buffer(&data, size), ec);

    if (ec)
    {
        std::cout << "Failed writing";
        return EXIT_FAILURE;
    }
    if (size != bytesWritten)
    {
        std::cout << "Size and written bytes differ";
        return EXIT_FAILURE;
    }
    
    std::cout << "Data has been successfully written: " << data;

    io.run();
    return 0;
}
