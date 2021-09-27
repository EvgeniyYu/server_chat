#include "../include/json_parser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <string>



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

void parse_user_list_to_json(const std::vector<std::string>& v_users, std::string& str_json)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ptree children;
	for (const auto& name: v_users)
	{
		boost::property_tree::ptree child;		
		child.put(MSG_USERNAME_STR, name);		
		children.push_back(std::make_pair("", child));
	}	
	pt.add_child(MSG_USERS_STR, children);
		
	std::ostringstream buf; 
	write_json (buf, pt, false);
	str_json = buf.str();

}
