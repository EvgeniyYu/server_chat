#ifndef SERVER_ENGINE_H
#define SERVER_ENGINE_H

#include "user_chat.h"
#include "json_parser.h"
#include "socketapi.h"
#include "def.h"
#include <memory>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <thread>


class ServerEngine
{
    Options mOptions;
    IServerSptr mServer_socket;
    boost::asio::thread_pool mPool;
    ChatSptr mChat;
public:
    ServerEngine(const Options& _options);
    ~ServerEngine();
private:
    void start_server_socket();
    void get_request(TypeFunc type, unsigned handler_session, const std::string& data);
};


#endif
