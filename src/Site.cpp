//
// Created by mas on 7/4/22.
//

#include "Site.h"
#include "Meta.h"
#include "Config.h"
#include <filesystem>
#include <fstream>

extern Meta *meta;
extern Config *config;

// static constant multiline string
const std::string htaccess = R"(
RewriteEngine On
RewriteCond %{REQUEST_FILENAME} !-d
        RewriteCond %{REQUEST_FILENAME} !-f
        RewriteRule ^([^\.]+)$ $1.html [NC, L]
)";

bool is_valid_page(const std::string &path)
{
    if (path.find("meta") != std::string::npos)
    {
        return false;
    }
    if (path.find("_imports") != std::string::npos)
    {
        return false;
    }
    if (path.ends_with(".html") || path.ends_with(".css") || path.ends_with(".js"))
    {
        return true;
    }
    return false;
}

bool is_copyable(const std::string &path)
{
    if (path.find("meta") != std::string::npos)
    {
        return false;
    }
    if (path.find("_imports") != std::string::npos)
    {
        return false;
    }
    if (path.ends_with(".md"))
    {
        return false;
    }
    return true;
}

Site::Site(std::string input_dir)
        : m_input_dir(std::move(input_dir))
{
    // open meta file
    std::string meta_path = m_input_dir + "/meta";
    meta = new Meta(meta_path);
    // recursively walk the input directory and build a list of pages
    for (auto &entry: std::filesystem::recursive_directory_iterator(m_input_dir))
    {
        if (!entry.is_directory() && is_valid_page(entry.path().string()))
        {
            m_pages.emplace_back(Page(entry.path().string()));
        }
        else if (!entry.is_directory() && is_copyable(entry.path().string()))
        {
            std::string output_path = config->get_output_dir() + entry.path().string().substr(m_input_dir.size());
            std::filesystem::create_directories(std::filesystem::path(output_path).parent_path());
            // overwrite existing files
            std::filesystem::copy_file(entry.path().string(), output_path, std::filesystem::copy_options::overwrite_existing);
        }
    }
}

void Site::write()
{
    for (auto &page: m_pages)
    {
        page.write();
    }
    // write out htaccess file
    std::ofstream file(config->get_output_dir() + "/.htaccess");
    file << htaccess;
    file.close();
}