#include "session.h"
#include "session_manager.h"
#include <iostream>
#include <random>
#include <algorithm>

Session::Session(tcp::socket socket, SessionManager& _manager): mSocket(std::move(socket)), mManager(_manager)
{
    mHandle = mManager.getRandomHandle();
}


Session::~Session()
{
}


void Session::start()
{
    mManager.receive_message(NEW_CONNECTION, mHandle, "");
    do_read();
}


void Session::add_msg_to_send(const std::string& msg)
{
    do_write(msg);
}

void Session::do_read()
{
    auto self(shared_from_this());
    mSocket.async_read_some(boost::asio::buffer(mData, mMax_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::string answ = std::string{mData, length};
                std::cout << "receive " << length << "=" << mData << "  handle = " << mSocket.native_handle() << std::endl;
                mManager.receive_message(GET_MESSAGE, mHandle, answ);
                do_read();
            }
            else
            {
                std::cout << "disconnect handle = " << mSocket.native_handle() << std::endl;
                mManager.receive_message(CLOSE_CONNECTION, mHandle, "");
                mManager.stop_session(mHandle);
            }
        });

}


void Session::do_write(const std::string& data)
{
    auto self(shared_from_this());
    boost::asio::async_write(mSocket, boost::asio::buffer(data, data.length()),
    [this, self, data](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                std::cout << "written: " << data << std::endl;
            }
            else
            {
                std::cout << "disconnect handle = " << mSocket.native_handle() << std::endl;
                mManager.receive_message(CLOSE_CONNECTION, mHandle, "");
                mManager.stop_session(mHandle);
            }
        });
}

