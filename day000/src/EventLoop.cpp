#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
EventLoop::EventLoop(/* args */) : ep(nullptr), bquilt(false)
{
    ep = new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep;
}

void EventLoop::loop()
{
    while (!bquilt)
    {
        std::vector<Channel *> chs;
        chs = ep->poll();
        for (auto it = chs.begin(); it != chs.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch)
{
    ep->updateChannel(ch);
}
