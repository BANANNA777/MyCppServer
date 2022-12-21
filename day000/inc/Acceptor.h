#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class SockAddress;
class Acceptor
{
private:
    /* data */
    EventLoop *loop;
    Socket *sock;
    Channel *acceptCh;
    SockAddress *addr;
    std::function<void(Socket *)> newConnectionCallback;

public:
    Acceptor(EventLoop *loop);
    ~Acceptor();
    void acceptConnection();
    int setNewConnectionCallback(std::function<void(Socket *sock)>);
};
