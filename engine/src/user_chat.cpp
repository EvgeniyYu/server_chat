#include "../include/user_chat.h"
#include "../include/json_parser.h"

UserChat::UserChat(std::shared_ptr<IServerAPI> _server_socket): server_socket(_server_socket)
{
	db = std::move(create_db());
	//read all rooms users: fill rooms
}

void UserChat::add_user(unsigned session_handle, User& u)
{
	std::unique_lock<std::mutex> lk(mx);
	users[session_handle] = u;
}


void UserChat::remove_user(unsigned session_handle)
{
	std::unique_lock<std::mutex> lk(mx);
	users.erase(session_handle);
}


void UserChat::deliver_message(unsigned session_handler, MessageData& msg)
{
	std::string str_json;
	switch (msg.type)
	{
		case GET_ALL_USERS:
			{
			std::string str_users = db->get_all_users();
			send_message(session_handler, str_users);
			}
			break;
		case GET_MESSAGES:
			{
			std::string str_messages = db->get_messages(users[session_handler].name);
			send_message(session_handler, str_messages);
			}
			break;
		case SEND_ROOM_MSG:
			//realize
			break;
		case SEND_USER_MSG:
			{
			bool found = false;
			for (auto it = users.begin(); it != users.end(); ++it)
			{
				if (it->second.name == msg.user_name_to)
				{
					parse_msg_to_json(msg, str_json);	
					send_message(it->first, str_json);
					found = true;
					break;			
				}
			}
			if (!found)
			{
				//save message to db for user_name_to 
				db->insert_new_message(msg.user_name_to, str_json);
			}
			}
			break;
		default:
			break;
	}
}


void UserChat::send_message(unsigned session_handler, const std::string& str_msg)
{
	server_socket->send_message(session_handler, str_msg);
}

bool UserChat::check_user_authorized(unsigned session_handler, std::string& name, const std::string& auth_number)
{			
	//read password for the user_name from DataBase
	DBUser dbuser;
	json_string data_json = db->get_user_data(name);					
	parse_json_to_dbuser(data_json, dbuser);
	//check hash password for authorization
	bool result = check_password(users[session_handler].auth_number, dbuser.password, auth_number);
	if (result && users[session_handler].name.empty())
	{
		users[session_handler].name = name;
		users[session_handler].password = dbuser.password;
	}	
	return result;
}


bool UserChat::check_password(std::string key, std::string password, const std::string& encrypted_password)
{
	if (!key.size())
		return false;

	//password encryption
	for (size_t i = 0; i < password.size(); ++i)
	{
		password[i] ^= key[i % key.size()];
	}
	//std::cout << "encryption password: " << password << std::endl; 
	return (password == encrypted_password)? true: false;
}
