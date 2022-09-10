//
// Created by mas on 7/4/22.
//

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include <utility>
#include <sstream>

#include "Page.h"
#include "Config.h"
#include "Block.h"
#include "Meta.h"
#include "Markdown.h"

extern Config *config;
extern Meta *meta;

Page::Page(std::string path)
        : m_path(std::move(path)), m_lastmodified(0)
{
    m_output_path = config->get_output_dir() + m_path.substr(config->get_input_dir().size());
    m_final_path = m_output_path.substr(config->get_output_dir().size());
}

Page::Page(std::string path, std::shared_ptr<Template> templ, std::string slot)
        : m_path(std::move(path)), m_template(std::move(templ)), m_slot(std::move(slot)), m_lastmodified(0)
{
    std::string filepath = m_path.substr(config->get_input_dir().size());
    // replace .md with .html if it's a markdown file
    if (filepath.find(".md") != std::string::npos)
    {
        filepath.replace(filepath.find(".md"), 3, ".html");
    }
    // strip file extension
    filepath = filepath.substr(0, filepath.find_last_of('.')) + ".html";
    m_output_path = config->get_output_dir() + filepath;
    m_final_path = filepath;

    Logger::log("Rendering templated page: " + m_path);
}

// copy constructor
Page::Page(const Page &page)
{
    m_path = page.m_path;
    m_output_path = page.m_output_path;
    m_final_path = page.m_final_path;
    m_raw = page.m_raw;
    m_rendered = page.m_rendered;
    m_template = nullptr;
    m_slot = page.m_slot;
    m_children = page.m_children;
    m_frontmatter = page.m_frontmatter;
    m_lastmodified = page.m_lastmodified;
}

// Destructor
Page::~Page() = default;

// loop over and render the markdown tags to create children
void Page::render_markdown_tags()
{
    // first loop over for IT_MARKDOWN
    size_t idx = 0;
    size_t last_idx = 0;
    std::stringstream ss;
    while ((idx = m_raw.find(Constants::IMPORT_TAGS[Constants::IT_MARKDOWN], idx)) != std::string::npos)
    {
        ss << m_raw.substr(last_idx, idx - last_idx);
        last_idx = m_raw.find(Constants::CLOSER, idx) + 1;
        std::string directory = utils::get_attribute(m_raw.substr(idx), "directory");
        std::string dir_path = utils::get_relative_path(m_path) + "/" + directory;
        std::string templ = utils::get_attribute(m_raw.substr(idx), "template");
        std::string templ_path = utils::get_relative_path(m_path) + "/" + templ;
        Template template_obj(templ_path);
        std::shared_ptr<Template> template_ptr = std::make_shared<Template>(template_obj);
        std::string slot = utils::get_attribute(m_raw.substr(idx), "slot");
        std::string sort_key = utils::get_attribute(m_raw.substr(idx), "sort");
        std::string title = utils::get_attribute(m_raw.substr(idx), "title");
        std::string order = utils::get_attribute(m_raw.substr(idx), "order");
        // iterate through files in directory
        for (auto &p: std::filesystem::directory_iterator(dir_path))
        {
            std::string file_path = p.path().string();
            if (p.path().extension() == ".md")
            {
                auto pg = Page(file_path, template_ptr, slot);
                pg.render();
                m_children.emplace_back(pg);

            }
        }
        if (order == "ascending" || order == "asc")
        {
            std::sort(m_children.begin(), m_children.end(), [&sort_key](Page &a, Page &b)
            {
                return a.get_frontmatter(sort_key) < b.get_frontmatter(sort_key);
            });
        }
        else if (order == "descending" || order == "desc")
        {
            std::sort(m_children.begin(), m_children.end(), [&sort_key](Page &a, Page &b)
            {
                return a.get_frontmatter(sort_key) > b.get_frontmatter(sort_key);
            });
        }
        else
        {
            std::cerr << "Warning: Invalid order attribute for markdown tag '" << order << "' in " << m_path
                      << std::endl;
        }
        ss << "<ul>\n";
        for (auto &child: m_children)
        {
            // get relative path to m_output_path
            std::string rel_path = child.get_final_path();
            // strip .html
            rel_path = rel_path.substr(0, rel_path.find_last_of('.'));
            ss << "<li><a href=\"" << rel_path << "\">" << child.get_frontmatter(title) << "</a></li>\n";
        }
        ss << "</ul>\n";
        idx = last_idx;
    }
    ss << m_raw.substr(last_idx);
    m_rendered = ss.str();
}

// render the file into the template
void Page::render_templating()
{

    // the file is templated, therefore we have to switch on it
    size_t idx = 0;
    std::unordered_map<std::string, std::string> blocks;
    std::string tmp = m_rendered;
    size_t tmpl_idx = tmp.find(Constants::OPENER);
    if (utils::identify_import(tmp, tmpl_idx) != Constants::IT_TEMPLATE)
    {
        std::cerr << "Warning: <bluesky-template /> must be the first bluesky tag in a file" << m_path << "."
                  << std::endl;
        std::cerr << tmp << std::endl;
        return;
    }
    // instantiate template shared_ptr
    std::string templ_path_raw = utils::get_attribute(tmp.substr(tmpl_idx), "template");
    std::string templ_path = utils::get_relative_path(m_path) + "/" + templ_path_raw;
    Template template_obj(templ_path);
    std::shared_ptr<Template> template_ptr = std::make_shared<Template>(template_obj);
    m_template = template_ptr;
    while ((idx = tmp.find(Constants::IMPORT_TAGS[Constants::IT_BLOCK], idx)) != std::string::npos)
    {
        std::string name = utils::get_attribute(tmp.substr(idx), "name");
        size_t tag_end = tmp.find(Constants::CLOSER, idx) + 1;
        size_t block_end_start = tmp.find(Constants::IMPORT_TAGS[Constants::IT_BLOCK_END], tag_end);
        size_t block_end_end = tmp.find(Constants::CLOSER, block_end_start + 1);
        auto block = Block(tmp.substr(tag_end, block_end_start - tag_end), m_path);
        blocks[name] = block.get_rendered();
        idx = block_end_end + 1;
    }
    m_rendered = m_template->render(blocks);
}

// render all variables of the form ${namespace.var}
void Page::render_variables()
{
    // replace variables
    size_t idx = 0;
    auto ss = std::stringstream();
    size_t last_idx = 0;
    while ((idx = m_rendered.find(Constants::VARIABLE_OPENER, idx)) != std::string::npos)
    {
        ss << m_rendered.substr(last_idx, idx - last_idx);
        size_t period = m_rendered.find('.', idx);
        size_t var_end = m_rendered.find(Constants::VARIABLE_CLOSER, period);
        last_idx = var_end + 1;
        // check if namespace is meta
        std::string ns = m_rendered.substr(idx + 2, period - idx - 2);
        if (ns == "meta")
        {
            std::string key = m_rendered.substr(period + 1, var_end - period - 1);
            if (meta->has(key))
            {
                ss << meta->get(key);
            }
            else
            {
                std::cerr << "Warning: meta key " << key << " does not exist for page" << m_path << std::endl;
            }
        }
        else
        {
            std::cerr << "Warning: Unknown namespace in variable " << m_rendered.substr(idx, var_end - idx + 1)
                      << "in page " << m_path << std::endl;
        }
        idx = last_idx;
    }
    ss << m_rendered.substr(last_idx);
    m_rendered = ss.str();
}

// Master render call, recursive
void Page::render()
{
    m_raw = utils::read_file(m_path);
    m_lastmodified = utils::get_last_modified(m_path);
    if (m_path.ends_with(".md"))
    {
        for (auto &pair: Markdown::parse_frontmatter(m_raw))
        {
            m_frontmatter[pair.first] = pair.second;
        }
        size_t frontmatter_end = Markdown::get_frontmatter_end(m_raw);
        m_raw = m_raw.substr(frontmatter_end);
    }

    if (!is_templated()) // the file is not templated, therefore we can directly render it
    {
        Logger::log("Page is not templated: " + m_path);
        m_rendered = Block(m_raw, m_path).get_rendered();
        Logger::log("Rendering variables: " + m_path);
        render_variables();
    }
    else if (m_path.ends_with(".md"))
    {
        Logger::log("Page is markdown: " + m_path);
        std::unordered_map<std::string, std::string> blocks;
        blocks[m_slot] = Markdown::parse(m_raw);
        m_rendered = m_template->render(blocks, m_frontmatter);
    }
    else
    {
        Logger::log("Page is templated: " + m_path);
        Logger::log("Rendering markdown tags...");
        render_markdown_tags();
        Logger::log("Rendering templating...");
        render_templating();
        Logger::log("Rendering variables...");
        render_variables();
    }
}

// write out the file and its children
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
    for (auto &child: m_children)
    {
        child.write();
    }
    if (!m_children.empty())
    {
        Logger::log("Wrote " + std::to_string(m_children.size()) + " children of " + m_path);
    }
}

// check if the page is templated
bool Page::is_templated()
{
    if (m_template != nullptr)
    {
        return true;
    }
    size_t idx = m_raw.find(Constants::OPENER, 0);
    if (idx == std::string::npos)
    {
        return false;
    }
    return utils::identify_import(m_raw, idx) == Constants::IT_TEMPLATE;
}

std::string Page::get_frontmatter(const std::string &key) const
{
    // print all keys and values
    if (m_frontmatter.find(key) != m_frontmatter.end())
    {
        return m_frontmatter.at(key);
    }
    else
    {
        Logger::warn("Warning: frontmatter key '" + key + "' not found in page '" + m_path + "'");
        return "";
    }
}