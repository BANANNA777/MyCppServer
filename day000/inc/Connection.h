#pragma once
#include <functional>
class EventLoop;
class Socket;
class Channel;
class SockAddress;
class Connection
{
private:
    /* data */
    EventLoop *loop;
    Socket *sock;
    SockAddress *addr;
    Channel *channel;
    
    typedef std::function<void(Socket *)> del_Callback ;
    del_Callback delCallback;
public:
    Connection(EventLoop*loop,Socket*sk);
    ~Connection();
    void echoET(int fd);
    void echoLT(int nfd);
    void setDelCallback(del_Callback);
};


