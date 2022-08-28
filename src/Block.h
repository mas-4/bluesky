//
// Created by mas on 7/10/22.
//

#ifndef BLUESKY_BLOCK_H
#define BLUESKY_BLOCK_H

#include <string>
#include "utils.h"


class Block
{
private:

    std::string m_raw;
    std::string m_rendered;
    std::string m_path;
    utils::SlotMap m_slots;
public:
    Block(std::string raw, std::string path);
    ~Block();
    std::string get_raw() { return m_raw; };
    std::string get_rendered() { return m_rendered; };
    void render();
    utils::SlotMap get_slots() { return m_slots; };
};

#endif //BLUESKY_BLOCK_H
