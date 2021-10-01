#ifndef REQUEST_EXECUTOR_H
#define REQUEST_EXECUTOR_H

#include "user_chat.h"
#include <memory>


class IRequest
{
protected:
    unsigned mSession_handler;
public:
    virtual void execute() = 0;
};

using IRequestUptr = std::unique_ptr<IRequest>;

class AddUser: public IRequest
{
    ChatSptr mChat;
public:
    AddUser(unsigned _session_handler, ChatSptr _chat);
    void execute() override ;
};

class RemoveUser: public IRequest
{
    ChatSptr mChat;
public:
    RemoveUser(unsigned _session_handler, ChatSptr _chat);
    void execute() override;
};

class HandleMessage: public IRequest
{
    const std::string str_json;
    ChatSptr mChat;
public:
    HandleMessage(unsigned _session_handler, const std::string& _str_json, ChatSptr _chat);
    void execute() override;
};

class WelcomeMessage: public IRequest
{
    ChatSptr mChat;
public:
    WelcomeMessage(unsigned _session_handler, ChatSptr _chat);
    void execute() override;
};


class RequestExecutor
{
    IRequestUptr mRequest;
    ChatSptr mChat;
    unsigned mSession_handler;
public:
    RequestExecutor(ChatSptr _chat);
    void set_request(TypeFunc type, unsigned _session_handler, const std::string& data);
    void execute();
};


#endif
