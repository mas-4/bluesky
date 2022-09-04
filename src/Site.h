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
public:
    explicit Site(std::string input_dir);

    ~Site() = default;

    void write();
};


#endif //BLUESKY_SITE_H
