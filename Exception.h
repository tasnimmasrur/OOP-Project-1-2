#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>

class MyException {
public:
    std::string msg;
    MyException(std::string m) : msg(m) {}
};
#endif
