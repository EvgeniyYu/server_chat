#include "socketapi.h"
#include "server.h"
#include <thread>

namespace socketapi
{


class ServerSocketAPI: public IServerAPI
{
    std::thread th;
    std::shared_ptr<Server> server;
    std::function<void(TypeFunc, unsigned, const std::string& data)> callback_msg;
public:
    ~ServerSocketAPI();
    void start_server(const size_t port) override;
    void stop_server() override;
    void send_message(unsigned handle, const std::string& data) override;

    void set_msg_callback(std::function<void(TypeFunc, unsigned, const std::string&)> _callback_msg) override;
    void receive_message(TypeFunc type, unsigned handle, const std::string& data) override;
};

ServerSocketAPI::~ServerSocketAPI()
{
    stop_server();
}

void ServerSocketAPI::start_server(const size_t port)
{
    server = std::make_shared<Server>(port);
    auto r_callback = std::bind(&ServerSocketAPI::receive_message,
                                                            this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2,
                                                            std::placeholders::_3);
    server->set_receive_callback(r_callback);
    th = std::thread(&Server::run, server);
}

void ServerSocketAPI::stop_server()
{
    th.join();
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


IServerUptr create_server_socket()
{
    return std::make_unique<ServerSocketAPI>();
}


}
