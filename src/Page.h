//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_PAGE_H
#define BLUESKY_PAGE_H

#include <string>
#include <map>
#include "Constants.h"


class Page
{
private:
    std::string m_path;
    std::string m_output_path;
    std::string m_raw;
    std::string m_rendered;
    time_t m_last_modified;
    time_t get_last_modified();
    ImportType identify_import(size_t idx);
    void read_file();
    bool exists();
public:
    explicit Page(std::string path);
    ~Page();
    void render();
    void write();
};

#endif //BLUESKY_PAGE_H
