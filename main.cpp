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
    Site site(config->get_input_dir());
    site.write();
    delete config;
}