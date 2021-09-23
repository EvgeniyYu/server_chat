#include "session.h"
#include "session_manager.h"
#include <iostream>
#include <random>
#include <algorithm>

Session::Session(tcp::socket socket, SessionManager& _manager): socket_(std::move(socket)), manager(_manager)
{	
	handle = manager.getRandomHandle();
}


Session::~Session()
{
}


void Session::start()
{
	manager.receive_message(NEW_CONNECTION, handle, "");	
	do_read();
}


void Session::add_msg_to_send(const std::string& msg) 
{
	do_write(msg);
}

void Session::do_read()
{
        auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
	{        
                if (!ec)
                {
                        std::string answ = std::string{data_, length};
                        std::cout << "receive " << length << "=" << data_ << "  handle = " << socket_.native_handle() << std::endl;
				manager.receive_message(GET_MESSAGE, handle, answ);								
				do_read();
                }
                else
                {
                        std::cout << "disconnect handle = " << socket_.native_handle() << std::endl;                
                        manager.receive_message(CLOSE_CONNECTION, handle, "");
                        manager.stop_session(handle);                     
                }                        
	});
             
}


void Session::do_write(const std::string& data)
{
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data, data.length()),
        [this, self, data](boost::system::error_code ec, std::size_t /*length*/)
        {
                if (!ec)
                {
          	        std::cout << "written: " << data << std::endl;
                }
                else
                {
          	        std::cout << "disconnect handle = " << socket_.native_handle() << std::endl;
          	        manager.receive_message(CLOSE_CONNECTION, handle, "");
          	        manager.stop_session(handle);          	
                }
        });
}

