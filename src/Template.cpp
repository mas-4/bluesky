//
// Created by Michael Sendker on 8/28/22.
//

#include "Template.h"
#include "utils.h"
#include "Block.h"
#include <sstream>
#include <iostream>

void Template::render()
{
    size_t idx = 0;
    size_t last_idx = 0;
    while ((idx = m_raw.find(Constants::IMPORT_TAGS[Constants::IT_SLOT], idx)) != std::string::npos)
    {
        // render previous block
        m_blocks.emplace_back(Block(m_raw.substr(last_idx, idx - last_idx), m_path).get_rendered());
        // save the slot
        m_blocks.emplace_back(utils::get_attribute(m_raw.substr(idx), "name"));
        // move on
        last_idx = m_raw.find(Constants::CLOSER, idx) + 1;
        idx = last_idx;
    }
    m_blocks.emplace_back(Block(m_raw.substr(last_idx), m_path).get_rendered());
}

Template::Template(std::string path)
: m_raw(utils::read_file(path)), m_path(std::move(path))
{
    std::cout << "Rendering template: " << m_path << std::endl;
    render();
}

std::string Template::render(const std::unordered_map<std::string, std::string> &blocks)
{
    std::stringstream ss;
    for (auto &block : m_blocks)
    {
        if (blocks.find(block) != blocks.end())
            ss << blocks.at(block);
        else
            ss << block;
    }
    return ss.str();
}
