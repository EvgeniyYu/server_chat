#ifndef COMMAND_QUEUE_H_INCLUDED
#define COMMAND_QUEUE_H_INCLUDED

#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <fstream>
#include <utility>

class Queue
{
	std::queue<std::pair<unsigned, std::string>> queue;
	std::mutex mx_ready_rw;
	std::condition_variable cv;
	std::ofstream ofs;
public:
	Queue() {}
	~Queue() {}

	size_t size() const;
	void push_back(std::pair<unsigned, std::string>& record);
	void get_front(std::pair<unsigned, std::string>& record);
};

#endif // COMMAND_QUEUE_H_INCLUDED
