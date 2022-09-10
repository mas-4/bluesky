#include <iostream>
#include "src/Config.h"
#include "src/Site.h"
#include "src/Logger.h"
#include "src/Server.h"

extern Config *config;

int main(int argc, char *argv[])
{
    config = new Config(argc, argv);
    Logger *logger = Logger::get_instance();
    logger->log("Starting BlueSky...");
    logger->log("Input directory: " + config->get_input_dir());
    logger->log("Output directory: " + config->get_output_dir());
    logger->log("Verbose: " + std::to_string(config->is_verbose()));
    logger->log("Serve: " + std::to_string(config->is_serve()));
    logger->log("Building site...");
    Site site(config->get_input_dir());
    if (config->is_serve())
    {
        logger->log("Serving site...");
        Server server(site, config->get_serve_ip());
        server.run();
    }
    else
    {
        logger->log("Writing site...");
        site.write();
    }
    delete config;
}