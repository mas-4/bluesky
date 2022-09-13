//
// Created by mas on 9/5/22.
//

#ifndef BLUESKY_LOGGER_H
#define BLUESKY_LOGGER_H

#include <string>

class Logger{
public:
    static void debug(const std::string &msg);
    static void warn(const std::string &msg);
    static void info(const std::string &msg);
    static void error(const std::string &msg);
};


#endif //BLUESKY_LOGGER_H
