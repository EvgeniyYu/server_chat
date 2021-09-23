#include "../include/server_engine.h"
#include "../include/request_executor.h"
#include <functional>


ServerEngine::ServerEngine(const size_t _port): port(_port)
{
	start_server_socket();
	chat = std::make_shared<UserChat>(server_socket);
}


ServerEngine::~ServerEngine()
{
}


void ServerEngine::start_server_socket()
{
	std::unique_ptr<IServerAPI> uptr_server = std::move(create_server_socket());
	server_socket = std::move(uptr_server);
	auto callback = std::bind(&ServerEngine::get_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	server_socket->set_msg_callback(callback);
	server_socket->start_server(port);
}


void ServerEngine::get_request(TypeFunc type, unsigned handler_session, const std::string& data)
{
	std::cout << "ServerEngine: type = " << type << "  handler = " << handler_session << "  data = " << data << std::endl; 

	boost::asio::post(pool, [this, type, handler_session, data]()
	{
		RequestExecutor req(chat);
		req.set_request(type, handler_session, data);		
	});
}
