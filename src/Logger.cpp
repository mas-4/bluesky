//
// Created by mas on 9/5/22.
//

#include "Logger.h"
#include "Config.h"

extern Config *config;

void Logger::debug(const std::string &msg)
{
    if (config->is_verbose())
    {
        std::cout << "DEBUG: " << msg << std::endl;
    }
}

void Logger::warn(const std::string &msg)
{
    // make console red
    std::cerr << "\033[1;31m" << "WARNING: " << "\033[0m";
    std::cerr << msg << std::endl;
}

void Logger::error(const std::string &msg)
{
    // make console red
    std::cerr << "\033[1;31m" << "ERROR: " << msg << "\033[0m" << std::endl ;
}

void Logger::info(const std::string &msg)
{
    std::cout << "INFO: " << msg << std::endl;
}