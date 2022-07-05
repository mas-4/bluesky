//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_CONSTANTS_H
#define BLUESKY_CONSTANTS_H

#include <string>

enum ImportType
{
    IT_INCLUDE,
    IT_TEMPLATE,
    IT_MARKDOWN,
    IT_PLACEHOLDER,
    IT_UNKNOWN
};


class Constants
{
public:
    static const std::string IMPORT_TAGS[4];
    static const std::string OPENER;
    static const std::string CLOSER;
    static const int IMPORT_TAG_LENGTH = 4;
};

#endif //BLUESKY_CONSTANTS_H
