#include "../include/server_engine.h"
#include <thread>
#include <chrono>


int main()
{
	ServerEngine engine(1234);
	
	while (1)
	{
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}
	return 0;
}

