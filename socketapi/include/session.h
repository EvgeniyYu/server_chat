#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <utility>
#include <boost/asio.hpp>


using boost::asio::ip::tcp;

class SessionManager;

class Session: public std::enable_shared_from_this<Session>
{
	tcp::socket socket_;
	SessionManager& manager;
	unsigned handle;
	enum { max_length = 4096 };
	char data_[max_length];
public:
	Session(tcp::socket socket, SessionManager& _manager);
	~Session();
	void start();
	void stop() {}
	unsigned get_handle() {return handle;}
	void add_msg_to_send(const std::string& msg);
private:
	void do_read();
	void do_write(const std::string& str);
};





#endif
