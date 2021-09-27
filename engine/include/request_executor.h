#ifndef REQUEST_EXECUTOR_H
#define REQUEST_EXECUTOR_H

#include "user_chat.h"
#include <memory>


class IRequest
{
protected:
	unsigned session_handler;
public:
	virtual void execute() = 0;
};

class AddUser: public IRequest
{
	std::shared_ptr<UserChat> chat;
public:
	AddUser(unsigned _session_handler, std::shared_ptr<UserChat> _chat);
	void execute() override ;
};

class RemoveUser: public IRequest
{
	std::shared_ptr<UserChat> chat;
public:
	RemoveUser(unsigned _session_handler, std::shared_ptr<UserChat> _chat);
	void execute() override; 
};

class HandleMessage: public IRequest
{
	const std::string str_json;
	std::shared_ptr<UserChat> chat;
public:
	HandleMessage(unsigned _session_handler, const std::string& _str_json, std::shared_ptr<UserChat> _chat);
	void execute() override; 
};

class WelcomeMessage: public IRequest
{
	std::shared_ptr<UserChat> chat;
public:
	WelcomeMessage(unsigned _session_handler, std::shared_ptr<UserChat> _chat);
	void execute() override; 
};

class RequestExecutor
{
	std::unique_ptr<IRequest> request;
	std::shared_ptr<UserChat> chat;
	unsigned session_handler;
public:
	RequestExecutor(std::shared_ptr<UserChat> _chat);
	void set_request(TypeFunc type, unsigned _session_handler, const std::string& data);
	void execute();
};






#endif
