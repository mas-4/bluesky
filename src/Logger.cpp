//
// Created by mas on 9/5/22.
//

#include "Logger.h"
#include "Config.h"

extern Config *config;

void Logger::log(const std::string &msg)
{
    if (config->is_verbose())
    {
        std::cout << msg << std::endl;
    }
}

void Logger::warn(const std::string &msg)
{
    std::cout << msg << std::endl;
}
