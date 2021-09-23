#include "session_manager.h"
#include <random>
#include <algorithm>
#include <iostream>

void SessionManager::start_session(std::shared_ptr<Session> sptr_session)
{
	sessions[sptr_session->get_handle()] = sptr_session;
	sptr_session->start();
}


void SessionManager::set_receive_callback(std::function<void(TypeFunc, unsigned, const std::string& data)> r_callback)
{
	receive_callback = r_callback;
}

void SessionManager::stop_session(unsigned handle)
{
	sessions[handle]->stop();
	sessions.erase(handle);
}

void SessionManager::stop_all_sessions()
{
	for (auto it = sessions.begin(); it != sessions.end(); ++it)
	{
		it->second->stop();
	}
	sessions.clear();
}

void SessionManager::send_message(unsigned handle, const std::string& msg)
{
	sessions[handle]->add_msg_to_send(msg);
}

void SessionManager::receive_message(TypeFunc type, unsigned handle, const std::string& msg)
{
	std::cout << "SessionManager::receive_message  type = " << type << "  handle: " << handle << "  data: " << msg << std::endl;
	receive_callback(type, handle, msg);
}

unsigned SessionManager::getRandomHandle()
{
        std::mt19937 g;
        unsigned value;
        while (1)
        {
    	        value = g();
    	        if (sessions.find(value) == sessions.end()) break;
	}
	return value;
}

