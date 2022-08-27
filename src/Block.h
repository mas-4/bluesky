//
// Created by mas on 7/10/22.
//

#ifndef BLUESKY_BLOCK_H
#define BLUESKY_BLOCK_H

#include <string>


class Block
{
private:

    std::string m_raw;
    std::string m_rendered;
    std::string m_path;
public:
    Block(std::string raw, std::string path);
    ~Block();
    std::string get_raw() { return m_raw; };
    std::string get_rendered() { return m_rendered; };
    void render();
};

#endif //BLUESKY_BLOCK_H
