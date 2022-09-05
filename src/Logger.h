//
// Created by mas on 9/5/22.
//

#ifndef BLUESKY_LOGGER_H
#define BLUESKY_LOGGER_H
#include <iostream>
#include <string>

class Logger{
    Logger() = default;
public:
    static Logger * get_instance(){
        static Logger instance;
        return &instance;
    }
    void log(const std::string &msg);
};


#endif //BLUESKY_LOGGER_H
