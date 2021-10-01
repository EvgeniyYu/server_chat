#include "../include/cmd_line_processor.h"
#include <boost/program_options.hpp>
#include <sstream>

namespace po = boost::program_options;


bool CommandLineProcessor::process_cmd_line(int argc, char** argv, std::ostream& ofs)
{
    try
    {
        po::options_description desc("Options");
        desc.add_options()
        ("help",     "help message")
        ("serverport",   po::value<unsigned>(&mOptions.server_port), "set the server port")
        ("dbhost",   po::value<std::string>(&mOptions.db_host), "set the database host")
        ("dbport",   po::value<unsigned>(&mOptions.db_port), "set the database port");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            ofs << desc << "\n";
            return false;
        }

        po::notify(vm);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

const Options& CommandLineProcessor::get_options() const
{
    return mOptions;
}


