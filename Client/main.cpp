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

    sock.async_connect(remote, [&sock, &msg, &msgSize](const boost::system::error_code& ec)
    {
        if (!ec)
        {
            async_read(sock, buffer(&msgSize, headerSize), [&sock, &msg, &msgSize](const boost::system::error_code& ec, std::size_t readBytes)
            {
                if (!ec)
                {
                    std::cout << "Header has been read successfully." << std::endl;
                    msg.resize(msgSize);

                    boost::system::error_code err;
                    std::size_t bytes = read(sock, buffer(msg, msgSize), err);

                    if (!err)
                    {
                        std::cout << "Body has been read successfully.";
                    }
                    else
                    {
                        std::cout << "Failed reading body.\n" << err.value() << ": " << err.message();
                    }
                    if (bytes != msgSize)
                    {
                        std::cout << "\nmsgSize and read bytes differ";
                    }
                }
                else
                {
                    std::cout << "Failed reading header.\n" << ec.value()  << ": " << ec.message();
                }
                if (headerSize != readBytes)
                {
                    std::cout << "\nExpected and read sizes differ";
                }
            });
        }
        else
        {
            std::cout << "Failed connection.\n" << ec.value() << ": " << ec.message();
        }
    });

    io.run();
    std::cout << msg;

    return 0;
}