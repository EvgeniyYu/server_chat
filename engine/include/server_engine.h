#ifndef SERVER_ENGINE_H
#define SERVER_ENGINE_H

#include "user_chat.h"
#include "json_parser.h"
#include "socketapi.h"
#include <memory>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <thread>


class ServerEngine
{
	const size_t port;
	std::shared_ptr<IServerAPI> server_socket;
	boost::asio::thread_pool pool;
	std::shared_ptr<UserChat> chat;
public:
	ServerEngine(const size_t _port);
	~ServerEngine();	
private:
	void start_server_socket();
	void get_request(TypeFunc type, unsigned handler_session, const std::string& data);
};


#endif
