#include "Client.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>

Client::Client(boost::asio::ip::tcp::endpoint remote)
    : _socket(_io, { boost::asio::ip::tcp::v4(), myPort })
{
    _socket.async_connect
    (
        remote, 
        boost::bind(&Client::connectionHandler, this, boost::asio::placeholders::error)
    );

    _io.run();
}

void Client::connectionHandler(const boost::system::error_code& ec)
{
    using namespace boost;
    if (!ec)
    {
        std::cout << "Connection established.";
        asio::async_read
        (
            _socket, 
            asio::buffer(&destination.size, headerSize), 
            boost::bind(&Client::headerHandler, this, asio::placeholders::error, asio::placeholders::bytes_transferred)
        );
    }
    else
    {
        std::cout << "Failed establishing connection.";
    }
    std::cout << std::endl;
}

void Client::headerHandler(const boost::system::error_code& ec, std::size_t bytes)
{
    using namespace boost;
    if (!ec)
    {
        std::cout << "Header has been read.";
        destination.buffer.resize(destination.size);
        asio::async_read
        (
            _socket, 
            asio::buffer(destination.buffer, destination.size), 
            boost::bind(&Client::bodyHandler, this, asio::placeholders::error, asio::placeholders::bytes_transferred)
        );
    }
    else
    {
        std::cout << "Failed reading header.";
    }
    std::cout << std::endl;
}

void Client::bodyHandler(const boost::system::error_code& ec, std::size_t bytes)
{
    using namespace boost;
    if (!ec)
    {
        std::cout << "Body has been read.";
    }
    else
    {
        std::cout << "Failed reading body.";
    }
    std::cout << std::endl;
}

void Client::showMessage() const
{
    std::cout << "Message size: " << destination.size
              << "\nContent: "    << destination.buffer;
}