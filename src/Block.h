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

    void render();

public:
    Block(std::string raw, std::string path);

    ~Block() = default;

    std::string get_rendered()
    { return m_rendered; };
};

#endif //BLUESKY_BLOCK_H
