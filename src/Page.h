//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_PAGE_H
#define BLUESKY_PAGE_H

#include <vector>
#include <string>
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
    Template *m_template = nullptr;
    bool is_templated();
    void render();

public:
    explicit Page(std::string path);
    Page(std::string path, Template *templ, std::string slot);
    ~Page();
    void write();
};

#endif //BLUESKY_PAGE_H
