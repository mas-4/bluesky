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
    long long last_slot_idx = -1;
    long long prev_slot_idx = -1;
    std::string last_slot;
    std::string prev_slot;
    size_t size_rendered = 0;
    // for slots we need (the last slot, the current slot, and the next slot)
    // for the last one we need the last slot, the current slot, and the end of the string
    while ((idx = m_raw.find(Constants::OPENER, idx)) != std::string::npos)
    {
        it = utils::identify_import(m_raw, idx);
        ss << m_raw.substr(last_idx, idx - last_idx);
        switch (it)
        {
            case Constants::IT_INCLUDE:
            {
                std::string name = utils::get_attribute(m_raw.substr(idx), "name");
                size_t tag_end = m_raw.substr(idx).find(Constants::CLOSER, idx) + 1;
                std::string path = utils::get_relative_path(m_path) + "/" + name;
                Page include = Page(path);
                include.render();
                auto rendered = include.get_rendered();
                size_rendered += rendered.size();
                ss << rendered;
                last_idx = tag_end;
                std::cout << "Include" << std::endl;
                break;
            }
            case Constants::IT_SLOT:
            {
                std::string name = utils::get_attribute(m_raw.substr(idx), "name");
                last_idx = m_raw.substr(idx).find(Constants::CLOSER, idx) + 1;
                if (last_slot_idx >= 0 && prev_slot_idx >= 0)
                {
                    m_slots[prev_slot] = std::make_tuple(prev_slot_idx, last_slot_idx, size_rendered);
                }
                prev_slot_idx = last_slot_idx;
                last_slot_idx = (int)size_rendered;
                prev_slot = last_slot;
                last_slot = name;
                std::cout << "Slot" << std::endl;
                break;
            }
            case Constants::IT_UNKNOWN:
            {
                size_t tag_end = m_raw.find(Constants::CLOSER, idx) + 1;
                std::string tag = m_raw.substr(idx, m_raw.find(Constants::CLOSER, idx) - idx + 1);
                std::cerr << "Unknown import type: " << tag << std::endl;
                last_idx = tag_end;
            }
            default:
                break;
        }
        idx++;
    }
    m_slots[prev_slot] = std::make_tuple(prev_slot_idx, last_slot_idx, size_rendered);
    ss << m_raw.substr(last_idx);
    m_rendered = ss.str();
    m_slots[last_slot] = std::make_tuple(last_slot_idx, size_rendered, m_rendered.size());
}

Block::~Block()
= default;
