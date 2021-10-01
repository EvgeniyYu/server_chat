#ifndef USER_CHAT_H
#define USER_CHAT_H

#include "socketapi.h"
#include "def.h"
#include "db.h"
#include <mutex>
#include <memory>
#include <map>
#include <random>

using namespace socketapi;
using namespace dbapi;

class UserChat
{
    std::mutex mx;
    std::shared_ptr<IServerAPI> mServer_socket;
    std::unique_ptr<IDatabaseAPI> mDb;
    std::map<unsigned, User> m_users;		//session_handle, user_name
    std::map<unsigned, std::vector<std::string>> v_rooms;	//room_id, vector_of_user_names
public:
    UserChat(std::shared_ptr<IServerAPI> _server_socket, const Options& options);
    void add_user(unsigned session_handle, User& u);
    void remove_user(unsigned session_handle);
    void deliver_message([[maybe_unused]]unsigned session_handler, MessageData& msg);
    void send_message(unsigned session_handler, const std::string& str_msg);
    bool check_user_authorized(unsigned session_handler, std::string& name, const std::string& auth_number);
private:
    bool check_password(std::string key, std::string password, const std::string& encrypted_password);
};


using ChatSptr = std::shared_ptr<UserChat>;




#endif
