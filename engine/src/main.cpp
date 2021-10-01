#include "../include/server_engine.h"
#include "../include/cmd_line_processor.h"
#include <thread>
#include <chrono>


int main(int argc, char *argv[])
{
    ICommandLineUptr uptr_cmdline = std::make_unique<CommandLineProcessor>();
    if (!uptr_cmdline->process_cmd_line(argc, argv, std::cout))
    {
        std::cerr << "Error parsing options in command line" << std::endl;
        return 1;
    }

    ServerEngine engine(uptr_cmdline->get_options());

    while (1)
    {
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
    return 0;
}

