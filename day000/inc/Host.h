#pragma once
#include <vector>
#include <sys/epoll.h>
class Socket;
class Epoll;
class Channel;
class Host
{
private:
    /* data */
    // Socket *sock;
    bool bETMode;
   // std::vector<Socket *> arrSock;
public:
    Host(/* args */);
    ~Host();
    int StartClient();
    int StartServer();
    int StartServer2();

    Socket *GetSock(int fd);
    int ClrALlSock();

private:
    int DoET(Socket *sk, Epoll *ep, epoll_event *events, int nfds);
    int DoLT(Socket *sk, Epoll *ep, epoll_event *events, int nfds);

    int DoET2(Socket *sk, Epoll *ep, std::vector<Channel*> vCh);
    int DoLT2(Socket *sk, Epoll *ep, std::vector<Channel*> vCh);
};
