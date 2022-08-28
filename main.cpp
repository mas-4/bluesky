#include <iostream>
#include "src/Config.h"
#include "src/Site.h"

extern Config *config;

int main(int argc, char *argv[])
{
    config = new Config(argc, argv);
    std::cout << config->m_input_dir << std::endl;
    std::cout << config->m_output_dir << std::endl;
    std::cout << config->m_serve_ip << std::endl;
    std::cout << "Verbose: " << config->m_verbose << std::endl;
    std::cout << "Force: " << config->m_force << std::endl;
    Site site(config->m_input_dir);
    site.write();
    delete config;
}