//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_PAGE_H
#define BLUESKY_PAGE_H

#include <vector>
#include <string>
#include <memory>

#include "Constants.h"
#include "Page.h"
#include "Block.h"
#include "Template.h"
#include "utils.h"


class Page
{
private:
    std::string m_path;
    std::vector<Page> m_children;
    std::string m_output_path;
    std::string m_raw;
    std::string m_slot;
    std::string m_rendered;
    std::shared_ptr<Template> m_template;
    bool is_templated();
    void render();

public:
    explicit Page(std::string path);
    Page(const Page &page); // copy constructor
    Page(std::string path, std::shared_ptr<Template> templ, std::string slot);
    ~Page() = default;
    void write();
};

#endif //BLUESKY_PAGE_H
