#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <Common/Common.hpp>


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

    Header data = 232;
    std::size_t size = sizeof(Header);

    acc.async_accept([&connections, &data, &size](const error_code& ec, socket sock)
    {
        if (!ec)
        {
            async_write(sock, buffer(&data, size), [&size, &data](const boost::system::error_code& ec, std::size_t writtenBytes)
            {
                if (ec)
                {
                    std::cout << "Failed writing";
                }
                if (size != writtenBytes)
                {
                    std::cout << "Size and written bytes differ";
                }
                std::cout << "Data has been successfully written: " << data;
            });
            connections.emplace_back(std::move(sock));
        }
        else
        {
            std::cerr << "Failed connection";
        }
    });

    io.run();
    return 0;
}
