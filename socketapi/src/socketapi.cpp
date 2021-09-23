#include "socketapi.h"
#include <thread>

void ServerSocketAPI::start_server(const size_t port)
{
	server = new Server(port);
	auto r_callback = std::bind(&ServerSocketAPI::receive_message, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	server->set_receive_callback(r_callback);
	
	std::thread th(&Server::run, server);
	th.detach();
}

void ServerSocketAPI::stop_server()
{
	if (server)
		delete server;
}

void ServerSocketAPI::send_message(unsigned handle, const std::string& data)
{
	if (server)
		server->send_message(handle, data);
}

void ServerSocketAPI::set_msg_callback(std::function<void(TypeFunc, unsigned, const std::string&)> _callback_msg)
{
	callback_msg = _callback_msg;
}

void ServerSocketAPI::receive_message(TypeFunc type, unsigned handle, const std::string& data)
{
	callback_msg(type, handle, data);
}


std::unique_ptr<IServerAPI> create_server_socket()
{
	return std::make_unique<ServerSocketAPI>();
}

