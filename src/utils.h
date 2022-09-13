//
// Created by mas on 7/10/22.
//

#ifndef BLUESKY_UTILS_H
#define BLUESKY_UTILS_H

#include "Constants.h"

#include <unordered_map>

namespace utils
{
    Constants::ImportType identify_import(const std::string &raw, size_t idx);

    [[maybe_unused]]
    time_t get_last_modified(const std::string &path);

    std::string get_relative_path(const std::string &path);

    std::string get_final_path(const std::string &parent_path, const std::string &path);

    std::string get_attribute(const std::string &line, const std::string &attribute);

    std::string read_file(const std::string &path);
}


#endif //BLUESKY_UTILS_H
