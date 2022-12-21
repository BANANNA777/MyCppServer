#pragma once
//#include <iostream>
#include <string>
class Buffer
{
private:
    /* data */
    std::string buf;
    
public:
    Buffer(/* args */);
    ~Buffer();
    void append(const char* buf,int bufsize);
    void clear();
    ssize_t size();
    const char* getchar();

};
