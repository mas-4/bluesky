//
// Created by Michael Sendker on 8/28/22.
//

#include "Block.h"
#include "Meta.h"
#include "Logger.h"
#include "Template.h"
#include "utils.h"
#include <sstream>
#include <iostream>

extern Meta *meta;

std::string
Template::render_variables(const std::string &block, const std::unordered_map<std::string, std::string> &frontmatter)
{
    // replace variables
    size_t idx = 0;
    auto ss = std::stringstream();
    size_t last_idx = 0;
    while ((idx = block.find(Constants::VARIABLE_OPENER, idx)) != std::string::npos)
    {
        ss << block.substr(last_idx, idx - last_idx);
        size_t period = block.find('.', idx);
        size_t var_end = block.find(Constants::VARIABLE_CLOSER, period);
        last_idx = var_end + 1;
        // check if namespace is meta
        std::string ns = block.substr(idx + 2, period - idx - 2);
        if (ns == "meta")
        {
            std::string key = block.substr(period + 1, var_end - period - 1);
            if (meta->has(key))
            {
                ss << meta->get(key);
            }
            else
            {
                Logger::warn("Meta key " + key + " not found.");
            }
        }
        else if (ns == "frontmatter")
        {
            std::string key = block.substr(period + 1, var_end - period - 1);
            if (frontmatter.contains(key))
            {
                ss << frontmatter.at(key);
            }
            else
            {
                Logger::warn("Frontmatter key " + key + " not found.");
            }
        }
        else
        {
            Logger::warn("Unknown namespace " + ns + ".");
        }
        idx = last_idx;
    }
    ss << block.substr(last_idx);
    return ss.str();
}

void Template::render()
{
    size_t idx = 0;
    size_t last_idx = 0;
    while ((idx = m_raw.find(Constants::IMPORT_TAGS[Constants::IT_SLOT], idx)) != std::string::npos)
    {
        // render previous block
        m_blocks.push_back(Block(m_raw.substr(last_idx, idx - last_idx), m_path).get_rendered());
        // save the slot
        m_blocks.push_back(utils::get_attribute(m_raw.substr(idx), "name"));
        // move on
        last_idx = m_raw.find(Constants::CLOSER, idx) + 1;
        idx = last_idx;
    }
    m_blocks.push_back(Block(m_raw.substr(last_idx), m_path).get_rendered());
}

Template::Template(std::string path)
        : m_raw(utils::read_file(path)), m_path(std::move(path))
{
    render();
}

std::string Template::render(
        const std::unordered_map<std::string, std::string> &blocks,
        const std::unordered_map<std::string, std::string> &frontmatter
)
{
    std::stringstream ss;
    for (auto &block: m_blocks)
    {
        // markdown blocks
        if (blocks.find(block) != blocks.end())
        {
            ss << blocks.at(block);
        }
        else
        { // template blocks
            ss << render_variables(block, frontmatter);
        }
    }
    return ss.str();
}

std::string Template::render(const std::unordered_map<std::string, std::string> &blocks)
{
    std::stringstream ss;
    for (auto &block: m_blocks)
    {
        if (blocks.find(block) != blocks.end())
        { // markdown blocks
            ss << blocks.at(block);
        }
        else
        { // template blocks
            ss << block;
        }
    }
    return ss.str();
}
