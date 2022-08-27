//
// Created by mas on 7/10/22.
//

#include "utils.h"
#include <iostream>

Constants::ImportType
utils::identify_import(const std::string &raw, size_t idx)
{
    for (int i = 0; i < Constants::IMPORT_TAG_LENGTH; i++)
        if (raw.substr(idx, Constants::IMPORT_TAGS[i].length()) == Constants::IMPORT_TAGS[i])
            return (Constants::ImportType) i;
    return Constants::IT_UNKNOWN;
}

[[maybe_unused]]
time_t get_last_modified(const std::string &path)
{
    struct stat st{};
    stat(path.c_str(), &st);
    return st.st_mtime;
}

[[maybe_unused]]
std::string get_relative_path(const std::string &path)
{
    return path.substr(0, path.find_last_of('/'));
}

[[maybe_unused]]
std::string get_attribute(const std::string &line, const std::string &attribute)
{
    size_t start = line.find(attribute);
    if (start == std::string::npos)
    {
        std::cerr << "Error: failed to find attribute " << attribute << " in "
                  << line << std::endl;
        exit(1);
    }
    start += attribute.length();
    size_t start_quote = line.find('"', start);
    size_t end_quote = line.find('"', start_quote + 1);
    return line.substr(start_quote + 1, end_quote - start_quote - 1);
}
