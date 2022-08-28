//
// Created by Michael Sendker on 8/28/22.
//

#ifndef BLUESKY_META_H
#define BLUESKY_META_H

#include <unordered_map>
#include <string>


class Meta
{
private:
    std::unordered_map<std::string, std::string> m_data;
public:
    explicit Meta(const std::string &path);
    ~Meta() = default;

    std::string get(const std::string &key);
    bool has(const std::string &key);
};


#endif //BLUESKY_META_H
