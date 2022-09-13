//
// Created by Michael Sendker on 8/28/22.
//

#include "Meta.h"

#include <fstream>
#include <utility>

Meta::Meta(const std::string &path)
{
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line))
    {
        std::string key = line.substr(0, line.find('='));
        std::string value = line.substr(line.find('=') + 1);
        m_data[key] = value;
    }

}

std::string Meta::get(const std::string &key)
{
    return m_data[key];
}

bool Meta::has(const std::string &key)
{
    return m_data.find(key) != m_data.end();
}

Meta *meta = nullptr;
