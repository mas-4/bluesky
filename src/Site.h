//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_SITE_H
#define BLUESKY_SITE_H

#include <vector>
#include "Page.h"

class Site
{
private:
    std::vector<Page> m_pages;
    std::string m_input_dir;
    std::unordered_map<std::string, Page*> m_pages_map;
public:
    explicit Site(std::string input_dir);

    ~Site() = default;

    void write();

    Page* get_page(const std::string& path) {
        return m_pages_map[path];
    };
};


#endif //BLUESKY_SITE_H
