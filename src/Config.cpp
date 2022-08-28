//
// Created by mas on 7/4/22.
//

#include "Config.h"
#include <unistd.h>
#include <iostream>

// create a null instance of Config to be initialized by main
Config *config = nullptr;

Config::Config(int argc, char *const argv[])
{
    int ch;
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
    // check if input directory is valid
    if (m_input_dir.empty()) {
        std::cerr << "Error: input directory is not specified" << std::endl;
        std::cerr << "Usage: " << argv[0] << " -i <input directory>" << std::endl;
        exit(1);
    } else if (access(m_input_dir.c_str(), F_OK) != 0) {
        std::cerr << "Error: input directory does not exist" << std::endl;
        exit(1);
    } else if (access(m_input_dir.c_str(), R_OK) != 0) {
        std::cerr << "Error: input directory is not readable" << std::endl;
        exit(1);
    }
}

Config::~Config()
= default;
