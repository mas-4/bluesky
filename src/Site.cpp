//
// Created by mas on 7/4/22.
//

#include "Site.h"
#include "Meta.h"
#include "Config.h"
#include "Logger.h"
#include "utils.h"
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
        Logger::debug("Opening meta file: " + meta_path);
    }
    meta = new Meta(meta_path);
    // recursively walk the input directory and build a list of pages
    generate();
}

void Site::generate()
{

    m_pages.clear();
    m_pages_map.clear();
    m_files_map.clear();
    m_files_time_map.clear();
    for (auto &entry: std::filesystem::recursive_directory_iterator(m_input_dir))
    {
        if (!entry.is_directory())
        { m_files_time_map[entry.path()] = utils::get_last_modified(entry.path()); }
        if (!entry.is_directory() && is_valid_page(entry.path().string()))
        {
            Logger::debug("Adding page " + entry.path().string());
            auto p = Page(entry.path().string());
            p.render();
            m_pages.emplace_back(p);
        }
        else if (!entry.is_directory() && is_copyable(entry.path().string()))
        {
            Logger::debug("Copying " + entry.path().string());
            std::string final_path = entry.path().string().substr(m_input_dir.size());
            std::string output_path = config->get_output_dir() + final_path;
            std::filesystem::create_directories(std::filesystem::path(output_path).parent_path());
            // overwrite existing files
            std::filesystem::copy_file(entry.path().string(),
                                       output_path,
                                       std::filesystem::copy_options::overwrite_existing);
            // read the file into m_file_map
            m_files_map[final_path] = utils::read_file(entry.path().string());
        }
    }
    Logger::debug("Added " + std::to_string(m_pages.size()) + " pages");
    for (auto &page: m_pages)
    {
        m_pages_map[page.get_final_path()] = page.get_rendered();
        Logger::debug("Added page " + page.get_final_path());
        for (auto &child: page.get_children())
        {
            m_pages_map[child.get_final_path()] = child.get_rendered();
            Logger::debug("Added child " + child.get_final_path());
        }
    }
}

void Site::write()
{
    for (auto &page: m_pages)
    {
        Logger::debug("Writing page " + page.get_path());
        page.write();
    }

    Logger::debug("Wrote " + std::to_string(m_pages.size()) + " top level pages");
    Logger::debug("Writing .htaccess");
    Logger::debug("Writing " + config->get_output_dir() + "/.htaccess");
    // write out htaccess file
    std::ofstream file(config->get_output_dir() + "/.htaccess");
    file << htaccess;
    file.close();
}

bool Site::has_page(const std::string &path) const
{
    return m_pages_map.find(path) != m_pages_map.end();
}

bool Site::has_file(const std::string &path) const
{
    return m_files_map.find(path) != m_files_map.end();
}

void Site::rerender()
{
    for (auto &entry: std::filesystem::recursive_directory_iterator(m_input_dir))
    {
        std::string path_s = entry.path().string();
        bool requires_regen = !m_files_time_map.contains(entry.path()) ||
                              m_files_time_map[entry.path()] < utils::get_last_modified(entry.path());
        if (!entry.is_directory() && requires_regen)
        {
            Logger::info("Re-rendering due to " + path_s);
            generate();
            return;
        }
    }
    for (auto &pair: m_files_time_map)
    {
        if (!std::filesystem::exists(pair.first))
        {
            Logger::info("Re-rendering due to " + pair.first);
            generate();
            return;
        }
    }
}