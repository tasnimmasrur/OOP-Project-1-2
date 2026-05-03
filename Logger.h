#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <iostream>

class Logger {
public:
    void log(const std::string& msg) const {
        std::cout << "[LOG]: " << msg << std::endl;
    }
};
#endif
