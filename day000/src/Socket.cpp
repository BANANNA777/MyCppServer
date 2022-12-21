#include "Socket.h"
#include "SockAddress.h"
#include "util.h"

#include <arpa/inet.h> 
#include <strings.h> 
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

Socket::Socket():sockfd(-1)
{
    sockfd = socket(AF_INET, SOCK_STREAM,0);
    errinfo(sockfd==-1,"socket err");
    //printf("struct %d\n",sockfd);
    //Socket(AF_INET, SOCK_STREAM,0);
}
Socket::Socket(int fd):sockfd(fd)
{
     errinfo(sockfd==-1,"socket err");
}

Socket::Socket(int __domain, int __type, int __protocol)
{
    sockfd = socket(__domain,__type,__protocol);
    errinfo(sockfd==-1,"socket err");
    //printf("struct %d\n",sockfd);
}

Socket::~Socket()
{
   if(sockfd!=-1)
   {
     close();
     sockfd =-1;
   }
   //printf("socket fd%d destruct\n",sockfd);
}

int Socket::connect(const SockAddress*addr)
{
    if (!addr)
    {
        return 0;
    }
    errinfo(::connect(sockfd, (sockaddr *)&addr->addr, sizeof(addr->addr)) == -1, "client connect error");
    return 0;
}

int Socket::setsockop()
{
    int on = 1;
    errinfo(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0, "socket op err");
    return 0;
}

int Socket::setnonblocking()
{
    errinfo(fcntl(sockfd, F_SETFL, fcntl(sockfd,F_GETFL)|O_NONBLOCK),"socket nonblocking error");
    return 0;
}

int Socket::bind(const SockAddress * addr)
{
    if (!addr)
    {
        return 0;
    }
    errinfo(::bind(sockfd, (sockaddr *)&addr->addr, sizeof(addr->addr)) == -1, "server bind error");
   return 0;
}

int Socket::accept(const SockAddress * addr)
{
    socklen_t len = sizeof(addr->addr);
    return ::accept(sockfd, (sockaddr *)&addr->addr, &len);
}

int Socket::listen()
{
    errinfo(::listen(sockfd, SOMAXCONN) == -1, "server listen error");
    return 0;
}

ssize_t Socket::write(char *buff, int size)
{
    return ::write(sockfd, buff, size);
    //return 0;
}

ssize_t Socket::read(char *buff, int size)
{   
    return ::read(sockfd,buff,size);
    //return 0;
}

int Socket::close()
{
    ::close(sockfd);
    printf("socket fd%d close\n",sockfd);
    return 0;
}
