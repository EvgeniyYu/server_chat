#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <string>

#include <thread>
#include <iostream>
#include <string.h>

#define MSG_TYPE_STR		"Type"
#define MSG_TIME_STR		"Time"
#define MSG_USER_FROM_STR	"Username_from"
#define MSG_USER_TO_STR		"User_to"
#define MSG_AUTH_NUM_STR	"Auth_number"
#define MSG_TEXT_STR		"Text"
#define MSG_USERNAME_STR	"Name"
#define MSG_USERS_STR		"Users"


namespace ba = boost::asio;

enum TypeMsg {                  DEFAULT, 		//by default
				GET_AUTH_NUM,		//get authentification number	
				LOGIN, 			//request (authentification), reply(login is required)  
				AUTH_OK,		//reply (authentification succeeded)
				AUTH_FAILED, 		//reply (authentification failed)
				GET_ALL_USERS, 		//get all users
				GET_MESSAGES, 		//get unread messages for the user
				SEND_ROOM_MSG, 		//send message to the room
				SEND_USER_MSG};		//send message to the user

struct MessageData
{
	std::string time;
	std::string text;
	std::string user_name_from;
	std::string user_name_to;
	unsigned room_id = 0;
	std::string auth_number;
	TypeMsg type = DEFAULT;
};


void parse_json_to_msg(const std::string& str_json, MessageData& msg) 
{
	boost::property_tree::ptree pt;
	std::istringstream isstr(str_json);
	read_json(isstr, pt);
	msg.time = pt.get<std::string>(MSG_TIME_STR);
	int type = pt.get<int>(MSG_TYPE_STR);
	msg.type = (TypeMsg)type;
	msg.user_name_from = pt.get<std::string>(MSG_USER_FROM_STR);
	msg.user_name_to = pt.get<std::string>(MSG_USER_TO_STR);
	msg.auth_number = pt.get<std::string>(MSG_AUTH_NUM_STR);
	msg.text = pt.get<std::string>(MSG_TEXT_STR);
}

void parse_msg_to_json(const MessageData& msg, std::string& str_result) 
{
	boost::property_tree::ptree pt;
	pt.put(MSG_TYPE_STR, msg.type);
	pt.put(MSG_TIME_STR, msg.time);
	pt.put(MSG_USER_FROM_STR, msg.user_name_from);
	pt.put(MSG_USER_TO_STR, msg.user_name_to);
	pt.put(MSG_AUTH_NUM_STR, msg.auth_number);
	pt.put(MSG_TEXT_STR, msg.text);
	std::ostringstream buf; 
	write_json (buf, pt, false);
	str_result = buf.str();
}


class ClientChat
{
	const std::string name;
	std::string auth_number;
	const std::string password;
	std::vector<std::string> users;
	char data[4096];
public:
	ClientChat(const std::string& _name, const std::string& _password): name(_name), password(_password)			
	{		
	}
	~ClientChat() {}
	void operator()()
	{				
		try 
		{	
			ba::io_context io_context;
			ba::ip::tcp::endpoint ep(
	                ba::ip::address::from_string("127.0.0.1"), 1234);
	                ba::ip::tcp::socket sock(io_context);
	                sock.connect(ep);
			size_t len;
			std::string str_json;
			
			len = sock.read_some(ba::buffer(data));
			std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
			
			std::cout << "SEND GET_AUTH_NUM" << std::endl;			
			str_json = prepare(GET_AUTH_NUM);
			ba::write(sock, ba::buffer(str_json, str_json.size()));        		
			len = sock.read_some(ba::buffer(data));
			std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
			analyze_answer(std::string{data, len});
			
			std::cout << "SEND LOGIN" << std::endl;
			str_json = prepare(LOGIN);
			ba::write(sock, ba::buffer(str_json, str_json.size()));        		
			len = sock.read_some(ba::buffer(data));
			std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
			analyze_answer(std::string{data, len});
			
			std::cout << "SEND GET_ALL_USERS" << std::endl;
			str_json = prepare(GET_ALL_USERS);	
			ba::write(sock, ba::buffer(str_json, str_json.size()));        		
			len = sock.read_some(ba::buffer(data));
			std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
			//analyze_answer(std::string{data, len});
			
			std::cout << "SEND GET_MESSAGES" << std::endl;
			str_json = prepare(GET_MESSAGES);	
			ba::write(sock, ba::buffer(str_json, str_json.size()));        		
			len = sock.read_some(ba::buffer(data));
			std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
			analyze_answer(std::string{data, len});
			
			std::cout << "SEND SEND_USER_MSG" << std::endl;
			str_json = prepare(SEND_USER_MSG);	
			ba::write(sock, ba::buffer(str_json, str_json.size()));        		
			//len = sock.read_some(ba::buffer(data));
			//std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
			//analyze_answer(std::string{data, len});
		}
		catch(const boost::system::system_error& ex) {std::cout << "boost exception! " << ex.what() << std::endl;}
    	        catch(const std::exception& ex) {std::cout << "std::exception! " << ex.what() << std::endl;}
	}
	
	std::string prepare(TypeMsg type)
	{
		MessageData msg;
		std::string str_json;
		msg.user_name_from = name;
		msg.type = type;
		msg.auth_number = auth_number;		
		switch (type)
		{	
			case GET_AUTH_NUM:									
			case GET_MESSAGES:
			case GET_ALL_USERS:
				parse_msg_to_json(msg, str_json);				
				break;
			case LOGIN:
				{
					std::string encoded_pwd;
					encode_password(auth_number, encoded_pwd);
					msg.auth_number = auth_number = encoded_pwd;
					parse_msg_to_json(msg, str_json);
				}
				break;
			case SEND_USER_MSG:
				msg.user_name_to = "Alex";
				msg.text = "Hello! How are you?";
				parse_msg_to_json(msg, str_json);
				break;
			default:
				return "";
		}
		return str_json;
	}
	
	void encode_password(const std::string& auth_number, std::string& encoded_pwd)
	{		
		if (auth_number.empty()) return;

		char* pwd = const_cast<char*>(password.c_str());

		for (size_t i = 0; i < strlen(pwd); ++i)
		{
			pwd[i] ^= auth_number[i % auth_number.size()];
		}
		std::cout << "encrypted password: " << pwd << std::endl;
		encoded_pwd = std::string(pwd);		
	}
	
	void analyze_answer(const std::string& str)
	{
		MessageData msg;
		parse_json_to_msg(str, msg);
		switch (msg.type)
		{
			case GET_AUTH_NUM:
				auth_number = msg.auth_number;
				std::cout << "AUTH NUMBER: " << auth_number << std::endl;
				break;
			case AUTH_OK:
				std::cout << "AUTH OK" << std::endl;
				break;
			case AUTH_FAILED:
				std::cout << "AUTH FAILED" << std::endl;
				exit(1);
				break;	
			case GET_ALL_USERS:				
				break;
			case GET_MESSAGES:
				std::cout << "MESSAGE: " << msg.text << std::endl;
				break;			
			default:
				break;
		}
	}
};


int main(int, char *[]) 
{
    std::locale::global(std::locale(""));
            
	ClientChat client("Evgeniy", "qwerty");		
	std::thread th(client);
	th.join();
    

    return 0;
}
