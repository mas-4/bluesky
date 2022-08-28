//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_PAGE_H
#define BLUESKY_PAGE_H

#include <string>
#include <unordered_map>
#include "Constants.h"
#include "Page.h"
#include "Block.h"
#include "Template.h"
#include "utils.h"


class Page
{
private:
    std::string m_path;
    std::string m_output_path;
    std::string m_raw;
    std::string m_rendered;
    Template *m_template;
    bool is_templated();
    void render();

public:
    explicit Page(std::string path);
    ~Page();
    std::string get_rendered() { return m_rendered; };
    void write();
};

#endif //BLUESKY_PAGE_H
