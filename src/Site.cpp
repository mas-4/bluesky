//
// Created by mas on 7/4/22.
//

#include "Site.h"
#include <filesystem>

Site::Site(std::string input_dir)
{
    m_input_dir = std::move(input_dir);
    m_pages = std::vector<Page>();
    // walk the input directory and build a list of pages
    for (const auto &entry : std::filesystem::directory_iterator(m_input_dir))
    {
        std::string path = entry.path().string();
        if (entry.is_regular_file())
        {
            // check that the file is not 'meta'
            if (path.find("meta") == std::string::npos)
            {
                m_pages.push_back(Page(path));
            }
        }
    }

}

Site::~Site()
{

}

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
