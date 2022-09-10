#include <iostream>
#include "src/Config.h"
#include "src/Site.h"
#include "src/Logger.h"
#include "src/Server.h"

extern Config *config;

int main(int argc, char *argv[])
{
    config = new Config(argc, argv);
    Logger::log("Starting BlueSky...");
    Logger::log("Input directory: " + config->get_input_dir());
    Logger::log("Output directory: " + config->get_output_dir());
    Logger::log("Verbose: " + std::to_string(config->is_verbose()));
    Logger::log("Serve: " + std::to_string(config->is_serve()));
    Logger::log("Building site...");
    Site site(config->get_input_dir());
    if (config->is_serve())
    {
        Logger::log("Serving site...");
        Server server(site, config->get_serve_ip());
        server.run();
    }
    else
    {
        Logger::log("Writing site...");
        site.write();
    }
    delete config;
}