#pragma once

#include <boost/asio.hpp>
#include <Common/Common.hpp>

class Client
{
    static constexpr std::uint16_t myPort = 64;

public:
    Client(boost::asio::ip::tcp::endpoint remote);

    void showMessage() const;

private:
    void connectionHandler(const boost::system::error_code& ec);
    void headerHandler(const boost::system::error_code& ec, std::size_t bytes);
    void bodyHandler(const boost::system::error_code& ec, std::size_t bytes);

    boost::asio::io_context      _io;
    boost::asio::ip::tcp::socket _socket;

    Message destination;
};
