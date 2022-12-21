#pragma once
class Epoll;
class Channel;
class EventLoop
{
private:
    /* data */
    Epoll*ep;
    bool bquilt;
public:
    EventLoop(/* args */);
    ~EventLoop();

    void loop();
    void updateChannel(Channel*ch);
};

