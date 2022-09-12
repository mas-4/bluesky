#include <iostream>
#include "src/Config.h"
#include "src/Site.h"
#include "src/Logger.h"
#include "src/Server.h"

extern Config *config;

int main(int argc, char *argv[])
{
    config = new Config(argc, argv);
    Logger::debug("Starting BlueSky...");
    Logger::debug("Input directory: " + config->get_input_dir());
    Logger::debug("Output directory: " + config->get_output_dir());
    Logger::debug("Verbose: " + std::to_string(config->is_verbose()));
    Logger::debug("Serve: " + std::to_string(config->is_serve()));
    Logger::debug("Building site...");
    Site site(config->get_input_dir());
    if (config->is_serve())
    {
        Logger::debug("Serving site...");
        Server server(site, config->get_serve_ip());
        server.run();
    }
    else
    {
        Logger::debug("Writing site...");
        site.write();
    }
    delete config;
}