#ifndef SERVER_H
#define SERVER_H

#include "session_manager.h"
#include <boost/asio.hpp>
#include <memory>
#include <utility>
//#include <map>
#include <set>





class Server
{
	boost::asio::io_context io_context;
	tcp::acceptor _acceptor;
	SessionManager manager;
public:
	Server(const size_t port);
	~Server();
	void run();
	void send_message(unsigned handle, const std::string& data);
	void set_receive_callback(std::function<void(TypeFunc, unsigned, const std::string& data)> r_callback);
private:
	void do_accept();
	void terminate();
};



#endif