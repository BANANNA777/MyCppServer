#pragma once
#include <arpa/inet.h> 
class SockAddress
{
//private:
public:
    /* data */
    sockaddr_in addr;
    socklen_t len;
public:
    SockAddress();
    ~SockAddress();

};


