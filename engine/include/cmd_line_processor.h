#ifndef CMD_LINE_PROCESSOR_H
#define CMD_LINE_PROCESSOR_H

#include "def.h"
#include <memory>
#include <iostream>


class ICommandLineProcessor
{
public:
    virtual bool process_cmd_line(int argc, char** argv, std::ostream& ofs) = 0;
    virtual const Options& get_options() const = 0;
};

class CommandLineProcessor: public ICommandLineProcessor
{
    Options mOptions;
public:
    bool process_cmd_line(int argc, char** argv, std::ostream& ofs) override;
    const Options& get_options() const override;
};

using ICommandLineUptr = std::unique_ptr<ICommandLineProcessor>;

#endif
