//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_PAGE_H
#define BLUESKY_PAGE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "Constants.h"
#include "Page.h"
#include "Block.h"
#include "Template.h"
#include "utils.h"
#include "Logger.h"


class Page
{
private:
    std::string m_filename;
    std::string m_path;
    std::string m_output_path;
    std::string m_final_path;
    std::string m_raw;
    std::string m_slot;
    std::string m_rendered;

    std::vector<Page> m_children;
    std::shared_ptr<Template> m_template;
    std::unordered_map<std::string, std::string> m_frontmatter;

    bool is_templated();

    void render();

    void render_markdown_tags();

    void render_templating();

    void render_variables();

public:
    explicit Page(std::string path);

    Page(const Page &page); // copy constructor
    Page(std::string path, std::shared_ptr<Template> templ, std::string slot);

    ~Page();

    void write();

    std::string get_out_path()
    { return m_output_path; };

    std::string get_path()
    { return m_path; };

    std::string get_frontmatter(const std::string &key) const
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
    };

    std::vector<Page> get_children()
    { return m_children; };

    std::string get_final_path() const
    { return m_final_path; };

    std::string get_rendered() const
    { return m_rendered; };

};

#endif //BLUESKY_PAGE_H
