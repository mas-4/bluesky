//
// Created by mas on 9/5/22.
//

#include "Config.h"
#include "Logger.h"

#include <iostream>
#include <cerrno>

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
    fprintf(stderr, "\033[1;31mERROR: %s: %m\n", msg.c_str());
}

void Logger::info(const std::string &msg)
{
    std::cout << "INFO: " << msg << std::endl;
}