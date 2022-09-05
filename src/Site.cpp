//
// Created by mas on 7/4/22.
//

#include "Site.h"
#include "Meta.h"
#include "Config.h"
#include "Logger.h"
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
    if (config->is_verbose())
    {
        std::cout << "Reading meta file from " << meta_path << std::endl;
    }
    meta = new Meta(meta_path);
    // recursively walk the input directory and build a list of pages
    for (auto &entry: std::filesystem::recursive_directory_iterator(m_input_dir))
    {
        if (!entry.is_directory() && is_valid_page(entry.path().string()))
        {
            Logger::get_instance()->log("Adding page " + entry.path().string());
            m_pages.emplace_back(Page(entry.path().string()));
        }
        else if (!entry.is_directory() && is_copyable(entry.path().string()))
        {
            Logger::get_instance()->log("Copying " + entry.path().string());
            std::string output_path = config->get_output_dir() + entry.path().string().substr(m_input_dir.size());
            std::filesystem::create_directories(std::filesystem::path(output_path).parent_path());
            // overwrite existing files
            std::filesystem::copy_file(entry.path().string(), output_path,
                                       std::filesystem::copy_options::overwrite_existing);
        }
    }
    Logger::get_instance()->log("Added " + std::to_string(m_pages.size()) + " pages");
}

void Site::write()
{
    for (auto &page: m_pages)
    {
        Logger::get_instance()->log("Writing page " + page.get_path());
        page.write();
    }

    Logger::get_instance()->log("Wrote " + std::to_string(m_pages.size()) + " top level pages");
    Logger::get_instance()->log("Writing .htaccess");
    std::cout << "Writing .htaccess" << std::endl;
    // write out htaccess file
    std::ofstream file(config->get_output_dir() + "/.htaccess");
    file << htaccess;
    file.close();
}