//
// Created by mas on 7/4/22.
//

#include "Config.h"
#include "Logger.h"

#include <iostream>
#include <unistd.h>

// create a null instance of Config to be initialized by main
Config *config = nullptr;

Config::Config(int argc, char *const argv[])
{
    int ch;
    const char *args = "i:o:h:vf";
    while ((ch = getopt(argc, argv, args)) != -1)
    {
        switch (ch)
        {
            case 'i':
                m_input_dir = std::string(optarg);
                break;
            case 'o':
                m_output_dir = std::string(optarg);
                break;
            case 'v':
                m_verbose = true;
                break;
            case 'h':
                m_serve_ip = std::string(optarg);
                break;
            default:
                break;
        }
    }
    // check if input directory is valid
    if (m_input_dir.empty())
    {
        std::cout << "Usage: " << argv[0] << " -i <input directory>" << std::endl;
        exit(1);
    }
    else if (access(m_input_dir.c_str(), F_OK) != 0)
    {
        Logger::error("input directory does not exist");
        exit(1);
    }
    else if (access(m_input_dir.c_str(), R_OK) != 0)
    {
        Logger::error("input directory is not readable");
        exit(1);
    }
    if (m_output_dir.empty())
    {
        m_output_dir = "./_build";
    }
}

const std::string &Config::get_input_dir() const
{
    return m_input_dir;
}

const std::string &Config::get_output_dir() const
{
    return m_output_dir;
}

const std::string &Config::get_serve_ip() const
{
    return m_serve_ip;
}

bool Config::is_verbose() const
{
    return m_verbose;
}

bool Config::is_serve() const
{
    return !m_serve_ip.empty();
}

Config::~Config()
= default;
