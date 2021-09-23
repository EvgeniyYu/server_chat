#include "../include/queue.h"


size_t Queue::size() const
{
    return queue.size();
}


void Queue::push_back(std::pair<unsigned, std::string>& record)
{
    {
        std::unique_lock<std::mutex> lk(mx_ready_rw);
		queue.push(record);
    }
    cv.notify_all();
}


void Queue::get_front(std::pair<unsigned, std::string>& record)
{
    {
		std::unique_lock<std::mutex> lk(mx_ready_rw);
		if (!queue.size())
		{
			cv.wait(lk);
		}
		record = queue.front();
		queue.pop();
    }
}
