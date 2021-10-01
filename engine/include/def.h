#ifndef DEF_H
#define DEF_H

#include <string>

struct Options
{
    unsigned server_port = 1234;
    std::string db_host = "localhost";
    unsigned db_port = 27017;
};

enum TypeMsg {
    DEFAULT,        //by default
    GET_AUTH_NUM,   //get authentification number
    LOGIN,          //request (authentification), reply(login is required)
    AUTH_OK,        //reply (authentification succeeded)
    AUTH_FAILED,    //reply (authentification failed)
    GET_ALL_USERS,  //get all users
    GET_MESSAGES,   //get unread messages for the user
    SEND_ROOM_MSG,  //send message to the room
    SEND_USER_MSG   //send message to the user
};

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


struct User
{
    std::string name;
    std::string password;
    std::string auth_number;			//random value
    User(std::string& _name): name(_name) {}
    User() {}
    User& operator=(const User& u)
    {
        name = u.name;
        password = u.password;
        auth_number = u.auth_number;
        return *this;
    }
};


#endif
