//
// Created by mas on 7/4/22.
//

#include "Config.h"
#include <unistd.h>
#include <cstring>

// create a null instance of Config to be initialized by main
Config *config = nullptr;

Config::Config(int argc, char *const argv[])
{
    int ch;
    size_t length;
    const char* args = "i:o:h:vf";
    while ((ch = getopt(argc, argv, args)) != -1) {
        switch (ch) {
            case 'i':
                m_input_dir = std::string(optarg);
                break;
            case 'o':
                m_output_dir = std::string(optarg);
                break;
            case 'v':
                m_verbose = true;
                break;
            case 'f':
                m_force = true;
                break;
            case 'h':
                m_serve_ip = std::string(optarg);
                break;
            default:
                break;
        }
    }

}

Config::~Config()
= default;
