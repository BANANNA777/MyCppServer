#pragma once

#include <sys/types.h>
#include <stdint.h>
#include <functional>
class Epoll;
class EventLoop;
class Channel
{
private:
    /* data */
    int fd;
    Epoll *ep;
    EventLoop *loop;
    uint32_t event;
    uint32_t revent;
    bool bInEpool;

    typedef std::function<void()> call_back;
    call_back callback2;

    std::function<void()> callback;

public:
    Channel(int fd, Epoll *ep);
    Channel(int fd, EventLoop *ep);
    ~Channel();

    int EnReadable(bool bET = true);
    int EnWriteable(bool bET = true);

    bool getInEpoll();
    int setInEpoll(bool bIn = true);

    uint32_t getEvent();
    int setREvent(uint32_t event);
    uint32_t getREvent();
    int getSockFd();

    int handleEvent();
    int handleEvent2();
    void setCallback(std::function<void()>);
    void setCallback2(call_back);
};
