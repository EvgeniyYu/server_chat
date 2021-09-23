#ifndef MESSAGING_H
#define MESSAGING_H

#include "session_manager.h"
#include "server.h"
#include <functional>
#include <memory>


class IServerAPI
{
public:
	virtual void start_server(const size_t) = 0;
	virtual void stop_server() = 0;
	virtual void send_message(unsigned, const std::string&) = 0;
	
	virtual void set_msg_callback(std::function<void(TypeFunc, unsigned, const std::string&)>) = 0;
	virtual void receive_message(TypeFunc, unsigned, const std::string&) = 0;
};


class ServerSocketAPI: public IServerAPI
{
	Server* server;
	std::function<void(TypeFunc, unsigned, const std::string& data)> callback_msg;
public:
	void start_server(const size_t port) override;
	void stop_server() override;
	void send_message(unsigned handle, const std::string& data) override;
	
	void set_msg_callback(std::function<void(TypeFunc, unsigned, const std::string&)> _callback_msg) override;
	void receive_message(TypeFunc type, unsigned handle, const std::string& data) override;
};

std::unique_ptr<IServerAPI> create_server_socket();



#endif
