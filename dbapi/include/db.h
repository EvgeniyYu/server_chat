#ifndef DB_H
#define DB_H

#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>
#include <mutex>
#include <time.h>
#include <iostream>
#include <string.h>
#include <memory>
#include <vector>

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

using json_string = std::string;

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
};

/*
The MongoDB C Driver is thread-unaware in the vast majority of its operations. 
This means it is up to the programmer to guarantee thread-safety.
*/
class MongoDB: public IDatabaseAPI
{
        std::mutex mx_query;
        const std::string database_name = "db_chat";
        const std::string user_collection_name = "coll_users";
        const std::string room_collection_name = "coll_rooms";
public:
	      MongoDB();
	      ~MongoDB();
	      bool insert_new_user(DBUser& user) override;	
	      bool remove_user(const std::string& name) override;	
	      void get_user_password(const std::string& name, std::string& password) override;
	      bool insert_new_message(const std::string& user_name_to, const std::string& str_msg) override;
	      void get_all_users(std::vector<std::string>& v_names) override;
	      void get_messages(const std::string& name, std::vector<std::string>& messages) override;
	      void remove_messages(const std::string& name) override;
private:	
        bool init_collection(const std::string& collection_name);
};


std::unique_ptr<IDatabaseAPI> create_db();


#endif
