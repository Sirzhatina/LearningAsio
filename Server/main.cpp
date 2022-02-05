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

    acc.async_accept([&connections, &msgSize, &moment, &msg](const error_code& ec, socket sock)
    {
        if (!ec)
        {
            connections.emplace_back(std::move(sock));
        
            moment = std::time(nullptr);
            msg = std::string("Connection established at ") + std::ctime(&moment);
            msgSize = msg.size();

            async_write(connections.back(), buffer(&msgSize, headerSize), [&connections, &sock, &msgSize, &msg](const error_code& ec, std::size_t writtenBytes)
            {
                if (!ec)
                {
                    std::cout << "Header has been successfully written." << std::endl;

                    async_write(connections.back(), buffer(msg, msgSize), [&msgSize](const error_code &ec, std::size_t bytes)
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
                    });
                }
                else
                {
                    std::cout << "Failed writing header\n" << ec.value() << ": " << ec.message();
                }
                if (headerSize != writtenBytes)
                {
                    std::cout << "\nSize and written bytes differ";
                }
            });
        }
        else
        {
            std::cerr << "Failed connection.\n" << ec.value() << ": " << ec.message();
        }
    });

    io.run();
    return 0;
}
