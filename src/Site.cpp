//
// Created by mas on 7/4/22.
//

#include "Site.h"
#include <filesystem>

Site::Site(std::string input_dir)
{
    m_input_dir = std::move(input_dir);
    m_pages = std::vector<Page>();
    // recursively walk the input directory and build a list of pages
    for (auto &entry : std::filesystem::recursive_directory_iterator(m_input_dir))
    {
        if (!entry.is_directory() && is_valid_page(entry.path().string()))
        {
            m_pages.emplace_back(Page(entry.path().string()));
        }
    }
}

Site::~Site()
= default;

void Site::render()
{
    for (auto &page : m_pages)
    {
        page.render();
    }
}

void Site::write()
{
    for (auto &page : m_pages)
    {
        page.write();
    }
}

bool Site::is_valid_page(const std::string& path)
{
    if (path.find("meta") != std::string::npos)
    {
        return false;
    }
    if (path.find("_imports") != std::string::npos)
    {
        return false;
    }
    return true;
}
