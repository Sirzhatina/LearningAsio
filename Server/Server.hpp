#pragma once

#include <vector>
#include <boost/asio.hpp>
#include <Common/Common.hpp>

class Server
{
    static constexpr std::uint16_t myPort = 25;
public:
    Server();

private:

    void acceptConnections();
    void headerHandler(const boost::system::error_code& ec, std::size_t bytes);
    void bodyHandler(const boost::system::error_code& ec, std::size_t bytes);

    std::vector<boost::asio::ip::tcp::socket> connections;

    boost::asio::io_context _io;
    boost::asio::ip::tcp::acceptor _acc;

    Message toSend;
};