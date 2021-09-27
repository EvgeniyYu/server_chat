#include "../include/request_executor.h"
#include "../include/def.h"
#include "../include/json_parser.h"




RequestExecutor::RequestExecutor(std::shared_ptr<UserChat> _chat): chat(_chat) 
{
}


void RequestExecutor::set_request(TypeFunc type, unsigned _session_handler, const std::string& data)
{
	session_handler = _session_handler;
	if (type == NEW_CONNECTION)
		request = std::make_unique<WelcomeMessage>(session_handler, chat);
	if (type == CLOSE_CONNECTION)
		request = std::make_unique<RemoveUser>(session_handler, chat);
	else if (type == GET_MESSAGE)
		request = std::make_unique<HandleMessage>(session_handler, data, chat);
}

void RequestExecutor::execute()
{
	request->execute();		
}


WelcomeMessage::WelcomeMessage(unsigned _session_handler, std::shared_ptr<UserChat> _chat): chat(_chat)
{
	this->session_handler = _session_handler;
}

void WelcomeMessage::execute()
{
	MessageData msg;
	msg.type = DEFAULT;
	msg.text = "Welcome to chat! Please login!";
	std::string str_result;	
	parse_msg_to_json(msg, str_result);
	chat->send_message(session_handler, str_result);
}


HandleMessage::HandleMessage(unsigned _session_handler, const std::string& _str_json, std::shared_ptr<UserChat> _chat): str_json(_str_json), chat(_chat) 
{
	this->session_handler = _session_handler;
}


void HandleMessage::execute()
{
	//handle message
	MessageData msg;
	User user;
	parse_json_to_msg(str_json, msg);
	if (msg.user_name_from.empty()) 
	{
		msg.type = AUTH_FAILED;
		std::string str_result;
		parse_msg_to_json(msg, str_result);
		chat->send_message(session_handler, str_result);
	}
	else
	{
		switch (msg.type)
		{
		case GET_AUTH_NUM:
			{							
				msg.type = GET_AUTH_NUM;
				std::mt19937 g;
				unsigned random_num = g();
				user.name = msg.user_name_from;
				user.auth_number = msg.auth_number = std::to_string(random_num);	//random number to send to user	 
				chat->add_user(session_handler, user);

				std::string str_result;	
				parse_msg_to_json(msg, str_result);
				chat->send_message(session_handler, str_result);

			}
			break;
		case LOGIN:
		{
			if (!chat->check_user_authorized(session_handler, msg.user_name_from, msg.auth_number))
			{
				msg.type = AUTH_FAILED;
			}
			else
			{
				user.name = msg.user_name_from;
				user.auth_number = msg.auth_number;
				msg.type = AUTH_OK;
				msg.text = "Welcome to chat!";
				//chat.update_user(session_handler, user);										
			}
			std::string str_result;
			parse_msg_to_json(msg, str_result);
			chat->send_message(session_handler, str_result);				
		}
			break;
		case GET_ALL_USERS:
		case GET_MESSAGES:
		case SEND_ROOM_MSG:
		case SEND_USER_MSG:
		{
			if (!chat->check_user_authorized(session_handler, msg.user_name_from, msg.auth_number))
			{
				msg.type = AUTH_FAILED;
			}
			else
			{
				chat->deliver_message(session_handler, msg);	
			}
		}
			break;
		default:
			break;
		}
	}
}


RemoveUser::RemoveUser(unsigned _session_handler, std::shared_ptr<UserChat> _chat): chat(_chat)
{
	this->session_handler = _session_handler;
}


void RemoveUser::execute()
{
	chat->remove_user(session_handler);
}


AddUser::AddUser(unsigned _session_handler, std::shared_ptr<UserChat> _chat): chat(_chat)
{
	this->session_handler = _session_handler;		
}


void AddUser::execute()
{
	//realize
}

