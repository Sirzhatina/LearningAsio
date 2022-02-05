#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <Common/Common.hpp>
#include <string>
#include <ctime>


int main(int argc, char* argv[])
{
    constexpr std::uint16_t port = 25;
    
    std::cout << "Server started!" << std::endl;
    using namespace boost::asio;

    io_context io;

    ip::tcp::acceptor acc(io, ip::tcp::endpoint{ ip::tcp::v4(), port });

    using socket     = ip::tcp::socket;
    using error_code = boost::system::error_code;

    std::vector<socket> connections;

    std::time_t moment;
    std::string msg;
    Header msgSize = 0;

    auto bodyHandler = [&msgSize](const error_code& ec, std::size_t bytes)
    {
        if (!ec)
        {
            std::cout << "Body has been successfully written.";
        }
        else
        {
            std::cout << "Failed writing body\n" << ec.value() << ": " << ec.message();
        }
        if (msgSize != bytes)
        {
            std::cout << "\nSize and written bytes of body differ";
        }
    };

    auto headerHandler = [&connections, &msgSize, &msg, bodyHandler](const error_code& ec, std::size_t bytes)
    {
        if (!ec)
        {
            std::cout << "Header has been successfully written." << std::endl;

            async_write(connections.back(), buffer(msg, msgSize), bodyHandler);
        }
        else
        {
            std::cout << "Failed writing header\n" << ec.value() << ": " << ec.message();
        }
        if (headerSize != bytes)
        {
            std::cout << "\nSize and written bytes differ";
        }
    };

    auto acceptorHandler = [&connections, &msgSize, &moment, &msg, &headerHandler](const error_code& ec, socket sock)
    {
        if (!ec)
        {
            moment = std::time(nullptr);
            connections.emplace_back(std::move(sock));
        
            msg = std::string("Connection established at ") + std::ctime(&moment);
            msgSize = msg.size();

            async_write(connections.back(), buffer(&msgSize, headerSize), headerHandler);
        }
        else
        {
            std::cerr << "Failed connection.\n" << ec.value() << ": " << ec.message();
        }
    };

    acc.async_accept(acceptorHandler);

    io.run();
    return 0;
}
