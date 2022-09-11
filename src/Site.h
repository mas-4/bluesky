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
    std::unordered_map<std::string, std::string> m_pages_map;
    std::unordered_map<std::string, std::string> m_files_map;
    std::unordered_map<std::string, time_t> m_files_time_map;
public:
    explicit Site(std::string input_dir);

    ~Site() = default;

    void write();

    [[nodiscard]] bool has_page(const std::string &path) const;
    [[nodiscard]] bool has_file(const std::string &path) const;

    void rerender();

    std::string get_page(const std::string &path)
    {
        return m_pages_map[path];
    };

    std::string get_file(const std::string &path)
    {
        return m_files_map[path];
    };

    void generate();
};


#endif //BLUESKY_SITE_H
