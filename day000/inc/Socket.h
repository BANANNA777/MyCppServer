#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include "util.h"
class SockAddress;
class Socket
{
private:
    /* data */
    int sockfd;

public:
    Socket();
    Socket(int fd);
    Socket(int __domain, int __type, int __protocol);

    ~Socket();

public:
    int getFd() { return sockfd; }
    int connect(const SockAddress *addr);
    int setsockop();
    int setnonblocking();
    int bind(const SockAddress *addr);
    int accept(const SockAddress *addr);
    int listen();
    ssize_t write(char *buff, int size);
    ssize_t read(char *buff, int size);
    int close();
};
