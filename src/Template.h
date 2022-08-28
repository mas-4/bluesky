//
// Created by Michael Sendker on 8/28/22.
//

#ifndef BLUESKY_TEMPLATE_H
#define BLUESKY_TEMPLATE_H

#include <vector>
#include <string>
#include <unordered_map>


class Template
{
private:
    std::string m_raw;
    std::string m_path;
    std::vector<std::string> m_blocks;
    void render();
public:
    explicit Template(std::string path);
    ~Template() = default;
    std::string render(const std::unordered_map<std::string, std::string> &blocks);
};


#endif //BLUESKY_TEMPLATE_H
