//
// Created by Michael Sendker on 9/1/22.
//

#ifndef BLUESKY_MARKDOWN_H
#define BLUESKY_MARKDOWN_H

#include <string>
#include <unordered_map>


namespace Markdown
{
    std::string parse(const std::string &raw);
    std::unordered_map<std::string, std::string> parse_frontmatter(const std::string &raw);
    size_t get_frontmatter_end(const std::string &line);
};


#endif //BLUESKY_MARKDOWN_H
