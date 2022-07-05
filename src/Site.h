//
// Created by mas on 7/4/22.
//

#ifndef SKYBLUE_SITE_H
#define SKYBLUE_SITE_H
#include <vector>
#include "Page.h"

class Site
{
private:
    std::vector<Page> m_pages;
    std::string m_input_dir;
    bool is_valid_page(std::string path);
public:
    Site(std::string input_dir);
    ~Site();

    void render();
    void write();
};


#endif //SKYBLUE_SITE_H
