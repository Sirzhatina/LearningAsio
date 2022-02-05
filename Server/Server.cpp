#include "Server.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>
#include <ctime>


Server::Server()
    : _acc(_io, { boost::asio::ip::tcp::v4(), boost::asio::ip::port_type(myPort) })
{

    acceptConnections();
    _io.run();
}

void Server::acceptConnections()
{
    _acc.async_accept([this](const boost::system::error_code& ec, boost::asio::ip::tcp::socket socket)
    {
        if (!ec)
        {
            std::time_t moment = std::time(nullptr);
            toSend.buffer = std::string("Connection established at ") + std::ctime(&moment);
            toSend.size = toSend.buffer.size();

            connections.push_back(std::move(socket));
            boost::asio::async_write
            (
                connections.back(), 
                boost::asio::buffer(&toSend.size, headerSize), 
                boost::bind(&Server::headerHandler, this, boost::placeholders::_1, boost::placeholders::_2)
            );

        }
        else
        {
            std::cout << "Failed accepting connection";
        }
    });
}

void Server::headerHandler(const boost::system::error_code& ec, std::size_t bytes)
{
    if (!ec)
    {
        boost::asio::async_write
        (
            connections.back(),
            boost::asio::buffer(toSend.buffer, toSend.size),
            boost::bind(&Server::bodyHandler, this, boost::placeholders::_1, boost::placeholders::_2)
        );
    }
    else
    {
        std::cout << "Failed writing header.";
    }
}

void Server::bodyHandler(const boost::system::error_code& ec, std::size_t bytes)
{
    if (!ec)
    {
        std::cout << "Message has been sent successfully.";
    }
    else
    {
        std::cout << "Failed writing body";
    }
}