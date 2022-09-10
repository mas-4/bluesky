//
// Created by mas on 7/10/22.
//

#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "sys/stat.h"
#include "Config.h"

extern Config *config;

Constants::ImportType utils::identify_import(const std::string &raw, size_t idx)
{
    for (int i = 0; i < Constants::IMPORT_TAG_LENGTH; i++)
    {
        std::string tag_str = raw.substr(idx, Constants::IMPORT_TAGS[i].length());
        if (tag_str == Constants::IMPORT_TAGS[i])
        {
            return (Constants::ImportType) i;
        }
    }
    return Constants::IT_UNKNOWN;
}

time_t utils::get_last_modified(const std::string &path)
{
    struct stat st{};
    stat(path.c_str(), &st);
    return st.st_mtime;
}

std::string utils::get_relative_path(const std::string &path)
{
    return path.substr(0, path.find_last_of('/'));
}

std::string utils::get_attribute(const std::string &line, const std::string &attribute)
{
    size_t start = line.find(attribute);
    if (start == std::string::npos)
    {
        std::cerr << "Warning: failed to find attribute " << attribute << " in "
                  << line << std::endl;
        return "";
    }
    start += attribute.length();
    size_t start_quote = line.find('"', start);
    size_t end_quote = line.find('"', start_quote + 1);
    return line.substr(start_quote + 1, end_quote - start_quote - 1);
}

std::string utils::read_file(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Error: failed to open file " << path << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

bool is_directory(const std::string &path)
{
    struct stat st{};
    stat(path.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

std::string utils::get_final_path(const std::string &parent_path, const std::string &path)
{
    // parent path: ./build/blog.hml
    // output_dir: ./build
    // our path: ./build/blog/test.html
    // strip the file name from the parent_path
    // check if parent_path is a filename or a directory

    std::string parent_dir = parent_path.substr(0, parent_path.find_last_of('/'));
    // remove the output dir from the parent path
    parent_dir = parent_dir.substr(config->get_output_dir().length());
    // strip the output dir from the path
    std::string final_path = path.substr(config->get_output_dir().length());
    // strip the parent dir from the path
    final_path = final_path.substr(parent_dir.length());
    // strip leading /
    if (final_path[0] == '/')
    {
        final_path = final_path.substr(1);
    }
    return final_path;
}
