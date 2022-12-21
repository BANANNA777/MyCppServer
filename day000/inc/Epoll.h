#pragma once
#include <sys/epoll.h>
#include <vector>
class Channel;
class Epoll
{
private:
    /* data */
    int epfd;
    struct epoll_event * events;
public:
    Epoll(/* args */);
    ~Epoll();
    int getEp(){return epfd;}
    int addEpFd(int sockfd, bool bET =true);
    int addEpFd(int sockfd, uint32_t ev);
    int getEvents(epoll_event *events, int timeout =-1);

    int updateChannel(Channel*channel);
    std::vector<Channel*> poll(int timeout =-1);
};

