//
// Created by mas on 7/4/22.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include "utils.h"
#include "Page.h"
#include "Config.h"
#include "Block.h"

extern Config *config;

Page::Page(std::string path)
        : m_path(std::move(path))
{
    m_output_path = config->m_output_dir + m_path.substr(config->m_input_dir.size());
    m_last_modified = utils::get_last_modified(m_path);
    Page::read_file();
}

void Page::read_file()
{
    std::ifstream file(m_path);
    if (!file.is_open())
    {
        std::cerr << "Error: failed to open file " << m_path << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    m_raw = buffer.str();
    file.close();
}


// This function is recursive
void Page::render()
{
    // render the file
    size_t idx = 0;
    Constants::ImportType it;
    size_t last_idx = 0;
    std::stringstream ss;
    while ((idx = m_raw.find(Constants::OPENER, idx)) != std::string::npos)
    {
        it = utils::identify_import(m_raw, idx);
        ss << m_raw.substr(last_idx, idx - last_idx);
        switch (it)
        {
            case Constants::IT_TEMPLATE:
            {
                std::string name = utils::get_attribute(m_raw.substr(idx), "name");
                size_t tag_end = m_raw.find(Constants::CLOSER, idx) + 1;
                std::string path = utils::get_relative_path(m_path) + "/" + name;
                m_template = new Page(path);
                m_template->render();
                last_idx = tag_end;
                break;
            }
            case Constants::IT_BLOCK:
            {
                std::string name = utils::get_attribute(m_raw.substr(idx), "name");
                size_t tag_end = m_raw.find(Constants::CLOSER, idx) + 1;
                size_t block_end_start = m_raw.find(Constants::IMPORT_TAGS[Constants::IT_BLOCK_END]);
                size_t block_end_end = m_raw.find(Constants::CLOSER, block_end_start + 1);
                std::string path = utils::get_relative_path(m_path) + "/" + name;
                m_blocks[name] = Block(m_raw.substr(tag_end, block_end_start - tag_end), path);
                last_idx = block_end_end;
                break;
            }
            case Constants::IT_SLOT:

            default:
                last_idx = idx;
                break;
        }
    }
}

void Page::write()
{
    // create the output directory if it doesn't exist
    std::string dir_path = m_output_path.substr(0, m_output_path.find_last_of('/'));
    std::filesystem::create_directories(dir_path);
    std::ofstream file(m_output_path);
    // check if the file exists
    if (!file.is_open())
    {
        std::cerr << "Could not open file " << m_output_path << std::endl;
        return;
    }
    file << m_rendered;
    file.close();
}
