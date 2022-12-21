#include "Epoll.h"
#include "Channel.h"
#include "util.h"

#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

#define MAX_EPOLL_EVENTS 1024
Epoll::Epoll(/* args */) : events(nullptr)
{
    epfd = epoll_create1(0);
    events = new epoll_event[MAX_EPOLL_EVENTS];
    bzero(events, sizeof(*events)*MAX_EPOLL_EVENTS);
}

Epoll::~Epoll()
{
    if (epfd!=-1)
    {
        epfd =-1;
        close(epfd);
        /* code */
    }
    delete []events;
}

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Epoll::addEpFd(int sockfd, bool bET)
{
    uint32_t ev = EPOLLIN;
    if (bET)
    {
        ev = ev | EPOLLET;
    }
    return addEpFd(sockfd, ev);
}
int Epoll::addEpFd(int sockfd, uint32_t ev1)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = ev1;
    ev.data.fd = sockfd;
    setnonblocking(sockfd);
    errinfo(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) == -1, "epoll_ctl error");
    return 0;
}
int Epoll::getEvents(epoll_event *events, int timeout)
{
    int nfds = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, timeout);
    return nfds;
}

std::vector<Channel *> Epoll::poll(int timeout)
{
    std::vector<Channel *> vCh;
    int nfds = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, timeout);
    errinfo(nfds == -1, "epoll_wait error");
 
    for (int i = 0; i < nfds; i++)
    {
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setREvent(events[i].events);
        vCh.push_back(ch);
    }
    return vCh;
}

int Epoll::updateChannel(Channel *channel)
{
    if (!channel)
    {
        return -1;
    }
    int nfd = channel->getSockFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = channel->getEvent();
    //ev.data.fd = nfd;
    ev.data.ptr = channel;
    if (channel->getInEpoll())
    {
        errinfo(epoll_ctl(epfd, EPOLL_CTL_MOD, nfd, &ev) == -1, "epoll_ctl error");
    }
    else
    {
        errinfo(epoll_ctl(epfd, EPOLL_CTL_ADD, nfd, &ev) == -1, "epoll_ctl error");
        channel->setInEpoll();
    }
    return 0;
}
