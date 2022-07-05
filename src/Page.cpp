//
// Created by mas on 7/4/22.
//

#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <filesystem>

#include "Page.h"
#include "Config.h"

extern Config *config;

Page::Page(std::string path)
        : m_path(std::move(path))
{
    m_output_path =
            config->m_output_dir + m_path.substr(config->m_input_dir.size());
    struct stat st;
    stat(m_path.c_str(), &st);
    m_last_modified = get_last_modified();

    // read the file
    Page::read_file();
}

time_t Page::get_last_modified()
{
    struct stat st;
    stat(m_path.c_str(), &st);
    return st.st_mtime;
}

void Page::read_file()
{
    // read the file
    std::ifstream file(m_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    m_raw = buffer.str();
    file.close();
}

ImportType Page::identify_import(size_t idx)
{
    for (int i = 0; i < Constants::IMPORT_TAG_LENGTH; i++)
    {
        bool match = m_raw.substr(
                idx,
                Constants::IMPORT_TAGS[i].length()
        ) == Constants::IMPORT_TAGS[i];
        if (match)
        {
            return (ImportType) i;
        }
    }
    return IT_UNKNOWN;
}

std::string get_relative_path(const std::string &path)
{
    return path.substr(0, path.find_last_of('/'));
}

// This function is recursive
void Page::render()
{
    if (m_last_modified != get_last_modified())
    {
        Page::read_file();
    }

    size_t idx = 0;
    ImportType it;
    std::stringstream ss;
    size_t last_idx = 0;
    while ((idx = m_raw.find(Constants::OPENER, idx)) != std::string::npos)
    {
        it = identify_import(idx);
        ss << m_raw.substr(last_idx, idx - last_idx);
        switch (it)
        {
            case IT_INCLUDE:
            {
                size_t name_start = m_raw.find("name=\"", idx);
                size_t name_end = m_raw.find('"', name_start + 6);
                size_t tag_end = m_raw.find(Constants::CLOSER, name_end) + 1;
                std::string name = m_raw.substr(name_start + 6,
                                                name_end - name_start - 6);
                std::string path = get_relative_path (m_path) + "/" + name;
                Page page = Page(path);
                page.render();
                ss << page.m_rendered;
                last_idx = tag_end;
                break;
            }
            case IT_TEMPLATE:
                break;
            case IT_MARKDOWN:
                break;
            case IT_PLACEHOLDER:
                break;
            case IT_UNKNOWN:
                std::cerr << "Unknown import type" << std::endl;
        }
        idx++;
    }
    ss << m_raw.substr(last_idx);
    m_rendered = ss.str();
}

void Page::write()
{
    // create the output directory if it doesn't exist
    std::string dir_path = m_output_path.substr(
            0,
            m_output_path.find_last_of('/')
    );
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

Page::~Page()
= default;
