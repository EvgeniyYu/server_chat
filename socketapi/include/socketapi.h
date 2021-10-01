#ifndef MESSAGING_H
#define MESSAGING_H

#include "session_manager.h"
#include <functional>
#include <memory>

/***********************************************************************************************************************
Interface class IServerAPI represents an interaction with a server for network messaging.
Functions:
void start_server(const size_t PORT) - runs server with a port value 'PORT'.
void stop_server() - terminates server.
void send_message(unsigned handle, const std::string& data) - sends data to a client with session value equal to handle
void set_msg_callback(std::function<void(TypeFunc, unsigned, const std::string&)>) - sets a callback to get response of clients from server.
void receive_message(TypeFunc, unsigned, const std::string&) - called by server to return client response.

Function create_server_socket() initiates an interaction with an asynchronous server Boost.Asio for network messaging.
***********************************************************************************************************************/

namespace socketapi
{

class IServerAPI
{
public:
    virtual void start_server(const size_t) = 0;
    virtual void stop_server() = 0;
    virtual void send_message(unsigned, const std::string&) = 0;

    virtual void set_msg_callback(std::function<void(TypeFunc, unsigned, const std::string&)>) = 0;
    virtual void receive_message(TypeFunc, unsigned, const std::string&) = 0;

    virtual ~IServerAPI() {}
};

using IServerUptr = std::unique_ptr<IServerAPI>;
using IServerSptr = std::shared_ptr<IServerAPI>;

IServerUptr create_server_socket();

}

#endif
