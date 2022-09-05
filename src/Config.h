//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_CONFIG_H
#define BLUESKY_CONFIG_H

#include <string>


class Config
{
private:
    std::string m_input_dir;
    std::string m_output_dir;
    std::string m_serve_ip;
    bool m_verbose = false;
    bool m_force = false;
public:
    Config(int argc, char *const *argv);

    ~Config();

        [[nodiscard]] const std::string &get_input_dir() const;

        [[nodiscard]] const std::string &get_output_dir() const;

        [[nodiscard]] const std::string &get_serve_ip() const;

        [[nodiscard]] bool is_verbose() const;

        [[nodiscard]] bool is_force() const;
};


#endif //BLUESKY_CONFIG_H
