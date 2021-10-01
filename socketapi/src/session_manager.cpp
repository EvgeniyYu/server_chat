#include "session_manager.h"
#include <random>
#include <algorithm>
#include <iostream>

void SessionManager::start_session(SessionSptr sptr_session)
{
    std::unique_lock<std::mutex> lk(mx_session);

    m_sessions[sptr_session->get_handle()] = sptr_session;
    sptr_session->start();
}


void SessionManager::set_receive_callback(std::function<void(TypeFunc, unsigned, const std::string& data)> r_callback)
{
    mReceive_callback = r_callback;
}

void SessionManager::stop_session(unsigned handle)
{
    std::unique_lock<std::mutex> lk(mx_session);

    m_sessions[handle]->stop();
    m_sessions.erase(handle);
}

void SessionManager::stop_all_sessions()
{
    std::unique_lock<std::mutex> lk(mx_session);

    for (auto it = m_sessions.begin(); it != m_sessions.end(); ++it)
    {
        it->second->stop();
    }
    m_sessions.clear();
}

void SessionManager::send_message(unsigned handle, const std::string& msg)
{
    std::unique_lock<std::mutex> lk(mx_session);

    m_sessions[handle]->add_msg_to_send(msg);
}

void SessionManager::receive_message(TypeFunc type, unsigned handle, const std::string& msg)
{
    mReceive_callback(type, handle, msg);
}

unsigned SessionManager::getRandomHandle()
{
    std::unique_lock<std::mutex> lk(mx_session);

    std::mt19937 g;
    unsigned value;
    while (1)
    {
        value = g();
        if (m_sessions.find(value) == m_sessions.end()) break;
    }
    return value;
}

