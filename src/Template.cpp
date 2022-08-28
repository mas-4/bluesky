//
// Created by Michael Sendker on 8/28/22.
//

#include "Template.h"
#include "utils.h"
#include "Block.h"
#include <sstream>

void Template::render()
{
    size_t idx = 0;
    size_t last_idx = 0;
    while ((idx = m_raw.find(Constants::OPENER, idx)) != std::string::npos)
    {
        Block block(m_raw.substr(last_idx, idx - last_idx), m_path);
        m_blocks.push_back(block.get_rendered());
        Constants::ImportType it = utils::identify_import(m_raw, idx);
        if (it == Constants::IT_SLOT)
        {
            std::string name = utils::get_attribute(m_raw.substr(idx), "name");
            m_blocks.push_back(name);
            last_idx = m_raw.find(Constants::CLOSER, idx) + 1;
        }
    }
    m_blocks.push_back(m_raw.substr(last_idx));
}

Template::Template(std::string path)
: m_raw(utils::read_file(path)), m_path(std::move(path))
{
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
