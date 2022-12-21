#include "Channel.h"

#include "Epoll.h"
#include "EventLoop.h"

Channel::Channel(int fd, Epoll *ep) : fd(fd), ep(ep), event(0), revent(0), bInEpool(false)
{
}

Channel::Channel(int fd, EventLoop *loop):fd(fd),loop(loop), event(0), revent(0), bInEpool(false)
{
}

Channel::~Channel()
{
}

int Channel::EnReadable(bool bET)
{
    event = EPOLLIN;
    if (bET)
    {
        event = event | EPOLLET;
    }
    loop->updateChannel(this);
    //ep->updateChannel(this);
    // bInEpool = true;
    return 0;
}

bool Channel::getInEpoll()
{
    return bInEpool;
}
int Channel::setInEpoll(bool bIn)
{
    bInEpool = bIn;
    return 0;
}
uint32_t Channel::getEvent()
{
    return event;
}
int Channel::setREvent(uint32_t event)
{
    revent = event;
    return 0;
}
uint32_t Channel::getREvent()
{
    return revent;
}
int Channel::getSockFd()
{
    return fd;
}

int Channel::handleEvent()
{
    callback();
    return 0;
}

int Channel::handleEvent2()
{
    callback2();
    return 0;
}

void Channel::setCallback(std::function<void()> cb)
{
    callback = cb;
}

void Channel::setCallback2(call_back cb)
{
    callback2 = cb;
}
