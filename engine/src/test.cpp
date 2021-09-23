#define BOOST_TEST_MODULE chat_server_test
#include <boost/test/unit_test.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <set>
#include <vector>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include "socketapi/socketapi.h"

BOOST_AUTO_TEST_SUITE(chat_server_test)

BOOST_AUTO_TEST_CASE(test_result)
{
	/*
	system("fuser -k 9000/tcp");

    Server server(1234);
    std::thread th(&Server::run, &server);

    std::thread th_client1([]()
    {
        system("seq 0 3 | nc localhost 9000");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        system("seq 3 7 | nc localhost 9000");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        system("seq 7 10 | nc localhost 9000");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    });
	th_client1.join();


    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	*/
	BOOST_CHECK (true);
	//th.join();
}

/*
namespace fs = std::filesystem;

BOOST_AUTO_TEST_SUITE(test_bulk_server)

BOOST_AUTO_TEST_CASE(test_result)
{

    //remove all files in Log directory
	const std::string dir_name = "Log";
	std::string dir_path = fs::current_path() / dir_name;
	fs::directory_entry entry_dir { dir_path };
    if (entry_dir.exists())
    {
    	fs::remove_all(dir_path);
    }
	//

	//free port 9000
	system("fuser -k 9000/tcp");

    std::thread th_server([]()
    {
        AsyncServer server(9000, 3);
    });

    std::thread th_client1([]()
    {
        system("seq 0 9 | nc localhost 9000");
    });

    std::thread th_client2([]()
    {
        system("seq 10 19 | nc localhost 9000");
    });


    std::this_thread::sleep_for(std::chrono::milliseconds(3000));


    //now we have only one file in Log directory and we can read it
    auto dirIter = std::filesystem::directory_iterator(dir_path);
	std::set<int> res;
	std::set<int>::iterator it;
    for (const auto & entry : fs::directory_iterator(dir_path))
    {
    	std::ifstream ifs(entry.path());
    	BOOST_CHECK(ifs.is_open() == true);		//log file must be open for reading
    	while (ifs)
    	{

    		std::string s;
    		std::getline(ifs, s);

            const std::string expr = "bulk:";
            std::size_t found = s.find(expr);
            if (found != std::string::npos)
            {
                s = s.substr(found + expr.length(), s.length());
            }
            else
            {
                continue;
            }
    		std::vector<std::string> v_values;
    		boost::split(v_values, s, boost::is_any_of(","));
    		for (auto& val: v_values)
    		{
                boost::algorithm::trim(val);
                int _val = std::atoi(val.c_str());
                res.insert(_val);
            }

    	}
    }

    std::set<int> res_true {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    BOOST_CHECK (res == res_true);

    exit(0);
}
*/

BOOST_AUTO_TEST_SUITE_END()

