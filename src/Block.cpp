//
// Created by mas on 7/10/22.
//

#include "Block.h"
#include "Constants.h"
#include "utils.h"
#include "Page.h"

#include <sstream>
#include <iostream>
#include <utility>

Block::Block(std::string raw, std::string path)
: m_raw(std::move(raw)), m_path(std::move(path))
{
    render();
}

void Block::render()
{

    size_t idx = 0;
    Constants::ImportType it;
    std::stringstream ss;
    size_t last_idx = 0;
    while ((idx = m_raw.find(Constants::OPENER, idx)) != std::string::npos)
    {
        it = utils::identify_import(m_raw, idx);
        ss << m_raw.substr(last_idx, idx - last_idx);
        switch (it)
        {
            case Constants::IT_INCLUDE:
            {
                std::string name = utils::get_attribute(m_raw.substr(idx), "name");
                size_t tag_end = m_raw.find(Constants::CLOSER, idx) + 1;
                std::string path = utils::get_relative_path(m_path) + "/" + name;
                Page include = Page(path);
                include.render();
                ss << include.get_rendered();
                last_idx = tag_end;
                break;
            }
            case Constants::IT_UNKNOWN:
            {
                std::string tag = m_raw.substr(
                        idx,
                        m_raw.find(Constants::CLOSER, idx) - idx + 1
                );
                std::cerr << "Unknown import type: " << tag << std::endl;
            }
            default:
                break;
        }
        idx++;
    }
    ss << m_raw.substr(last_idx);
    m_rendered = ss.str();
}

Block::~Block()
= default;
