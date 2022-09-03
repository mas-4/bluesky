//
// Created by mas on 7/4/22.
//

#include "Site.h"
#include "Meta.h"
#include <filesystem>

extern Meta *meta;

Site::Site(std::string input_dir)
        : m_input_dir(std::move(input_dir))
{
    // recursively walk the input directory and build a list of pages
    for (auto &entry: std::filesystem::recursive_directory_iterator(m_input_dir))
    {
        // check if filename is meta
        if (!entry.is_directory() && entry.path().filename() == "meta")
        {
            meta = new Meta(entry.path().string());
        }
        else if (!entry.is_directory() && is_valid_page(entry.path().string()))
        {
            m_pages.emplace_back(Page(entry.path().string()));
        }
    }
}

void Site::write()
{
    for (auto &page: m_pages)
    {
        page.write();
    }
}

bool Site::is_valid_page(const std::string &path)
{
    if (path.find("meta") != std::string::npos)
    {
        return false;
    }
    if (path.find("_imports") != std::string::npos)
    {
        return false;
    }
    if (path.find(".md") != std::string::npos)
    {
        return false;
    }
    return true;
}
