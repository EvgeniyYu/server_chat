#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "def.h"
#include "db.h"
#include <string>
#include <vector>

//message
#define MSG_TYPE_STR		"Type"
#define MSG_TIME_STR		"Time"
#define MSG_USER_FROM_STR	"Username_from"
#define MSG_USER_TO_STR		"User_to"
#define MSG_AUTH_NUM_STR	"Auth_number"
#define MSG_TEXT_STR		"Text"

#define MSG_USERNAME_STR	"Name"
#define MSG_USERS_STR		"Users"


//convert: json data <=> struct MessageData (for messaging with client)
void parse_json_to_msg(const std::string& str_json, MessageData& msg);
void parse_msg_to_json(const MessageData& msg, std::string& str_result);
void parse_user_list_to_json(const std::vector<std::string>& v_users, std::string& str_json);





#endif
