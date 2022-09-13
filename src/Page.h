//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_PAGE_H
#define BLUESKY_PAGE_H

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "Block.h"
#include "Constants.h"
#include "Logger.h"
#include "Page.h"
#include "Template.h"
#include "utils.h"


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
    time_t m_lastmodified;

    std::vector<Page> m_children;
    std::shared_ptr<Template> m_template;
    std::unordered_map<std::string, std::string> m_frontmatter;

    bool is_templated();


    void render_markdown_tags();

    void render_templating();

    void render_variables();

public:
    explicit Page(std::string path);

    Page(const Page &page); // copy constructor
    Page(std::string path, std::shared_ptr<Template> templ, std::string slot);

    ~Page();

    void render();

    void write();

    std::string get_path()
    { return m_path; };

    std::string get_frontmatter(const std::string &key) const;

    std::vector<Page> get_children()
    { return m_children; };

    std::string get_final_path() const
    { return m_final_path; };

    std::string get_rendered() const
    { return m_rendered; };

    time_t get_last_modified() const
    { return m_lastmodified; };

};

#endif //BLUESKY_PAGE_H
