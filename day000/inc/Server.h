#pragma once
#include <map>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    /* data */
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int,Connection*> mapConnection;
public:
    Server(EventLoop*loop);
    ~Server();

    void handleEventET(int);
    void handleEventLT(int);
    
    void newConnection(Socket*sock);
    void delConnection(Socket*sock);
};

