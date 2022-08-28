//
// Created by mas on 7/4/22.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include "Page.h"
#include "Config.h"
#include "Block.h"

extern Config *config;

Page::Page(std::string path)
        : m_path(std::move(path))
{
    m_output_path = config->m_output_dir + m_path.substr(config->m_input_dir.size());
    m_raw = utils::read_file(m_path);
    render();
}

// This function is recursive
void Page::render()
{
    if (!is_templated()) // the file is not templated, therefore we can directly render it
    {
        m_rendered = Block(m_raw, m_path).get_rendered();
        return;
    }

    // the file is templated, therefore we have to switch on it
    size_t idx = 0;
    Constants::ImportType it;
    std::unordered_map<std::string, std::string> blocks;
    size_t tmpl_idx = m_raw.find(Constants::OPENER);
    if (utils::identify_import(m_raw, idx) != Constants::IT_TEMPLATE)
    {
        std::cerr << "Error: <bluesky-template /> must be the first bluesky tag in a file" << m_path << "."
                  << std::endl;
        std::cerr << m_raw << std::endl;
        exit(1);
    }

    m_template = new Template(utils::get_relative_path(m_path) + "/" + utils::get_attribute(m_raw.substr(idx), "name"));
    while ((idx = m_raw.find(Constants::IMPORT_TAGS[Constants::IT_BLOCK], idx)) != std::string::npos)
    {
        std::string name = utils::get_attribute(m_raw.substr(idx), "name");
        size_t tag_end = m_raw.find(Constants::CLOSER, idx) + 1;
        size_t block_end_start = m_raw.find(Constants::IMPORT_TAGS[Constants::IT_BLOCK_END], tag_end);
        size_t block_end_end = m_raw.find(Constants::CLOSER, block_end_start + 1);
        auto block = Block(m_raw.substr(tag_end, block_end_start - tag_end), m_path);
        blocks[name] = block.get_rendered();
        idx = block_end_end + 1;
    }
    m_rendered = m_template->render(blocks);
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

bool Page::is_templated()
{
    size_t idx = m_raw.find(Constants::OPENER, 0);
    if (idx == std::string::npos)
    {
        return false;
    }
    return utils::identify_import(m_raw, idx) == Constants::IT_TEMPLATE;
}

Page::~Page()
{
    // TODO memory leak with template and new
    // Destructor is called multiple times on the same pointer, why?
    std::cout << (void*)m_template << std::endl;
    // delete m_template;
}
