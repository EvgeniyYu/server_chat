#ifndef MSG_QUEUE
#define MSG_QUEUE

#include <queue>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono> 

class MessageQueue
{
	std::queue<std::string> queue;
	std::mutex mx_ready_rw;
public:
	MessageQueue() 
	{
	}
	~MessageQueue() {}
	
	size_t size() 
	{
		std::unique_lock<std::mutex> lk(mx_ready_rw);
		return queue.size();
	}
	
	void push_back(const std::string& str)	//write to queue
	{
		std::unique_lock<std::mutex> lk(mx_ready_rw);
		queue.push(str);
	}
	
	
	bool get_front(std::string& str)	//read from a queue
	{	
					
		std::unique_lock<std::mutex> lk(mx_ready_rw);
		
		if (queue.size())
		{		
			str = queue.front();		
			queue.pop();
			return true;
		}
		return false;
	}	
	
};





#endif
