#include "server.h"
#include "session.h"

#include <iostream>
#include <thread>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;

Server::Server(const size_t port): _acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
}


void Server::run()
{
	do_accept();
	io_context.run();
}

void Server::terminate()
{
	_acceptor.close();
	manager.stop_all_sessions();
}

Server::~Server()
{
	terminate();
}

void Server::set_receive_callback(std::function<void(TypeFunc, unsigned, const std::string& data)> r_callback)
{
	manager.set_receive_callback(r_callback);
}

void Server::do_accept()
{
        _acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {    
            std::cout << "accept new connection handle = " << socket.native_handle() << std::endl;
            manager.start_session(std::make_shared<Session>(std::move(socket), manager));
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
	manager.send_message(handle, data);
}
