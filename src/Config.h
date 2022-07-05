//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_CONFIG_H
#define BLUESKY_CONFIG_H

#include <string>


class Config
{
public:
    Config(int argc, char *const *argv);
    ~Config();

    std::string m_input_dir;
    std::string m_output_dir;
    std::string m_serve_ip;
    bool m_verbose = false;
    bool m_force = false;
};


#endif //BLUESKY_CONFIG_H
