#include "../include/server_engine.h"
#include "../include/request_executor.h"
#include <functional>


ServerEngine::ServerEngine(const Options& _options): mOptions(_options)
{
    start_server_socket();
    mChat = std::make_shared<UserChat>(mServer_socket, mOptions);
}


ServerEngine::~ServerEngine()
{
}


void ServerEngine::start_server_socket()
{
    IServerUptr uptr_server = std::move(create_server_socket());
    mServer_socket = std::move(uptr_server);
    auto callback = std::bind(&ServerEngine::get_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    mServer_socket->set_msg_callback(callback);
    mServer_socket->start_server(mOptions.server_port);
}


void ServerEngine::get_request(TypeFunc type, unsigned handler_session, const std::string& data)
{
    boost::asio::post(mPool, [this, type, handler_session, data]()
    {
        RequestExecutor req(mChat);
        req.set_request(type, handler_session, data);
        req.execute();
    });
}
