#include "Acceptor.h"
#include "Socket.h"
#include "SockAddress.h"
#include "Channel.h"

#include <iostream>
Acceptor::Acceptor(EventLoop *loop) : loop(loop)
{
    sock = new Socket();
    addr = new SockAddress();
    sock->setsockop();
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();

    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);

    acceptCh = new Channel(sock->getFd(), loop);
    acceptCh->setCallback(cb);
    acceptCh->EnReadable();
}

Acceptor::~Acceptor()
{
}

void Acceptor::acceptConnection()
{
    SockAddress *addr = new SockAddress();
    Socket *client_sc = new Socket(sock->accept(addr));
    client_sc->setnonblocking();
    printf("client IP: %s PORT:%d fd:%d  connected with server \n",
           inet_ntoa(addr->addr.sin_addr), ntohs(addr->addr.sin_port), client_sc->getFd());
    newConnectionCallback(client_sc);
    delete addr;
}

int Acceptor::setNewConnectionCallback(std::function<void(Socket *sock)> cb)
{
    newConnectionCallback = cb;
    return 0;
}
