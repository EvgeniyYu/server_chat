#ifndef DB_H
#define DB_H

#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <memory>
#include <vector>

/****************************************************************************************************************************
Interface class IDatabaseAPI represents an interaction with database.
Functions
bool insert_new_user(DBUser& user) - inserts a new user.
bool remove_user(const std::string& name) - removes an user with a login equal to name.
bool get_user_password(const std::string& name, std::string& password) - returns user password.
bool insert_new_message(const std::string& user_name_to, const std::string& str_msg) - inserts a message for the user.
bool get_all_users(std::vector<std::string>& v_names) - returns all user names.
bool get_messages(const std::string& name, std::vector<std::string>& messages) - returns all message for the user.
bool remove_messages(const std::string& name) - clear all messages for the user.

Function create_db(host, port) initiates an interaction with MongoDB.
****************************************************************************************************************************/

namespace dbapi
{

struct Room
{
    unsigned id;
    std::string name;
    Room(unsigned _id, const std::string& _name = ""): id(_id), name(_name) {}
    Room() {}
};

struct Message
{
    struct tm mtime;
    std::string text;
    std::string author;
};

struct DBUser
{
    const std::string name;
    const std::string password;
    unsigned room_id = 0;
    std::vector<Room> rooms;
    std::vector<Message> messages;
    DBUser(const std::string& _name, const std::string& _password): name(_name), password(_password) {}
    DBUser() {}
};


class IDatabaseAPI
{
public:
    virtual bool insert_new_user(DBUser& user) = 0;
    virtual bool remove_user(const std::string& name) = 0;
    virtual void get_user_password(const std::string& name, std::string& password) = 0;
    virtual bool insert_new_message(const std::string& user_name_to, const std::string& str_msg) = 0;
    virtual void get_all_users(std::vector<std::string>& v_names) = 0;
    virtual void get_messages(const std::string& name, std::vector<std::string>& messages) = 0;
    virtual void remove_messages(const std::string& name) = 0;

    virtual ~IDatabaseAPI() {}
};

using IDatabaseUptr = std::unique_ptr<IDatabaseAPI>;

IDatabaseUptr create_db(const std::string host, const unsigned port);


}

#endif
