#include "server.h"
#include "session.h"

#include <iostream>
#include <thread>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;

//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------

Server::Server(const size_t port): mAcceptor(mIo_context, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server is running..." << std::endl;
}


void Server::run()
{
    do_accept();
    mIo_context.run();
}

void Server::terminate()
{
    mAcceptor.close();
    mManager.stop_all_sessions();
}

Server::~Server()
{
    terminate();
}

void Server::set_receive_callback(std::function<void(TypeFunc, unsigned, const std::string& data)> r_callback)
{
    mManager.set_receive_callback(r_callback);
}

void Server::do_accept()
{
    mAcceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "accept new connection handle = " << socket.native_handle() << std::endl;
                mManager.start_session(std::make_shared<Session>(std::move(socket), mManager));
            }
            else
            {
                std::cerr << "error code: " << ec.message() << std::endl;
            }

            do_accept();
        });
}


void Server::send_message(unsigned handle, const std::string& data)
{
    mManager.send_message(handle, data);
}
