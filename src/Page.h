//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_PAGE_H
#define BLUESKY_PAGE_H

#include <string>
#include <map>
#include "Constants.h"
#include "Page.h"
#include "Block.h"


class Page
{
private:
    std::string m_path;
    std::string m_output_path;
    std::string m_raw;
    std::string m_rendered;
    Page *m_template;
    time_t m_last_modified;
    void read_file();
    bool is_templated();

public:
    explicit Page(std::string path);
    ~Page() = default;
    void render();
    std::string get_raw() { return m_raw; };
    std::string get_rendered() { return m_rendered; };
    void write();
};

#endif //BLUESKY_PAGE_H
