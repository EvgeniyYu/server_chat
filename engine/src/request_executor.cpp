#include "../include/request_executor.h"
#include "../include/def.h"
#include "../include/json_parser.h"


RequestExecutor::RequestExecutor(ChatSptr _chat): mChat(_chat)
{
}

void RequestExecutor::set_request(TypeFunc type, unsigned _session_handler, const std::string& data)
{
    mSession_handler = _session_handler;
    if (type == NEW_CONNECTION)
        mRequest = std::make_unique<WelcomeMessage>(mSession_handler, mChat);
    else if (type == CLOSE_CONNECTION)
        mRequest = std::make_unique<RemoveUser>(mSession_handler, mChat);
    else if (type == GET_MESSAGE)
        mRequest = std::make_unique<HandleMessage>(mSession_handler, data, mChat);
}

void RequestExecutor::execute()
{
    mRequest->execute();
}


WelcomeMessage::WelcomeMessage(unsigned _session_handler, ChatSptr _chat): mChat(_chat)
{
    this->mSession_handler = _session_handler;
}

void WelcomeMessage::execute()
{
    MessageData msg;
    msg.type = DEFAULT;
    msg.text = "Welcome to chat! Please login!";
    std::string str_result;
    parse_msg_to_json(msg, str_result);
    mChat->send_message(mSession_handler, str_result);
}


HandleMessage::HandleMessage(unsigned _session_handler, const std::string& _str_json, ChatSptr _chat): str_json(_str_json), mChat(_chat)
{
    this->mSession_handler = _session_handler;
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
        mChat->send_message(mSession_handler, str_result);
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
                    mChat->add_user(mSession_handler, user);

                    std::string str_result;
                    parse_msg_to_json(msg, str_result);
                    mChat->send_message(mSession_handler, str_result);
                }
                break;
            case LOGIN:
                {
                    if (!mChat->check_user_authorized(mSession_handler, msg.user_name_from, msg.auth_number))
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
                    mChat->send_message(mSession_handler, str_result);
                }
                break;
            case GET_ALL_USERS:
            case GET_MESSAGES:
            case SEND_ROOM_MSG:
            case SEND_USER_MSG:
                {
                    if (!mChat->check_user_authorized(mSession_handler, msg.user_name_from, msg.auth_number))
                    {
                        msg.type = AUTH_FAILED;
                    }
                    else
                    {
                        mChat->deliver_message(mSession_handler, msg);
                    }
                }
                break;
            default:
                break;
        }
    }
}


RemoveUser::RemoveUser(unsigned _session_handler, ChatSptr _chat): mChat(_chat)
{
    this->mSession_handler = _session_handler;
}


void RemoveUser::execute()
{
    mChat->remove_user(mSession_handler);
}


AddUser::AddUser(unsigned _session_handler, ChatSptr _chat): mChat(_chat)
{
    this->mSession_handler = _session_handler;
}


void AddUser::execute()
{
    //realize
}

