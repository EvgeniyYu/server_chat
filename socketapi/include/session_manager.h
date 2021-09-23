#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <session.h>
#include <set>
#include <map>
#include <memory>

enum TypeFunc {NEW_CONNECTION, GET_MESSAGE, CLOSE_CONNECTION};

class SessionManager
{
	std::map<unsigned, std::shared_ptr<Session>> sessions;
	std::function<void(TypeFunc, unsigned, const std::string&)> receive_callback;
public:
	void start_session(std::shared_ptr<Session> sptr_session);
	void stop_session(unsigned handle);
	void stop_all_sessions();
	void send_message(unsigned handle, const std::string& msg);
	void receive_message(TypeFunc, unsigned, const std::string&);
	void set_receive_callback(std::function<void(TypeFunc, unsigned, const std::string& data)> r_callback);
	unsigned getRandomHandle();
};



#endif
