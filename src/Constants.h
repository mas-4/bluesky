//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_CONSTANTS_H
#define BLUESKY_CONSTANTS_H

#include <string>

namespace Constants
{

    enum ImportType
    {
        IT_INCLUDE,
        IT_TEMPLATE,
        IT_MARKDOWN,
        IT_SLOT,
        IT_BLOCK,
        IT_BLOCK_END,
        IT_UNKNOWN
    };
    static const size_t IMPORT_TAG_LENGTH = 6;
    static const std::string OPENER = "<bluesky-";
    static const std::string CLOSER = ">";
    static const std::string IMPORT_TAGS[IMPORT_TAG_LENGTH] = {
            "<bluesky-include",
            "<bluesky-template",
            "<bluesky-markdown",
            "<bluesky-slot",
            "<bluesky-block",
            "</bluesky-block>"
    };
    static const std::string TEMPLATE_END = "</bluesky-template>";


}

#endif //BLUESKY_CONSTANTS_H
