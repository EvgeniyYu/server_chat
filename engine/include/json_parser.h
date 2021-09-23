#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "def.h"
#include "db.h"
#include <string>

//message
#define MSG_TYPE_STR		"Type"
#define MSG_TIME_STR		"Time"
#define MSG_USER_FROM_STR	"Username_from"
#define MSG_USER_TO_STR		"User_to"
#define MSG_AUTH_NUM_STR	"Auth_number"
#define MSG_TEXT_STR		"Text"

//convert: json data <=> struct DBUser (for database)
void parse_json_to_dbuser([[maybe_unused]]const std::string& str_json, [[maybe_unused]]DBUser& user_result);
void parse_dbuser_to_json([[maybe_unused]]const DBUser& user_field, [[maybe_unused]]std::string& str_json_result);

//convert: json data <=> struct MessageData (for messaging with client)
void parse_json_to_msg(const std::string& str_json, MessageData& msg);
void parse_msg_to_json(const MessageData& msg, std::string& str_result);






#endif
