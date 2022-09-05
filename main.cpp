#include <iostream>
#include "src/Config.h"
#include "src/Site.h"

extern Config *config;

int main(int argc, char *argv[])
{
    config = new Config(argc, argv);
    if (config->is_verbose())
    {
        std::cout << "Input directory: " << config->get_input_dir() << std::endl;
        std::cout << "Output directory: " << config->get_output_dir() << std::endl;
        std::cout << "Serve IP: " << config->get_serve_ip() << std::endl;
    }
    if (config->is_verbose())
    {
        std::cout << "Building site..." << std::endl;
    }
    Site site(config->get_input_dir());
    if (config->is_verbose())
    {
        std::cout << "Writing site..." << std::endl;
    }
    site.write();
    delete config;
}