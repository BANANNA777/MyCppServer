#include "Server.h"
#include "Socket.h"
#include "SockAddress.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"

#include <functional>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#define WRITE_SERVER_BUFFER 1024
#define READ_SERVER_BUFFER 1024
Server::Server(EventLoop *loop) : loop(loop)
{

    // Socket *sock = new Socket();
    // SockAddress *addr = new SockAddress();
    // sock->setsockop();
    // sock->bind(addr);
    // sock->listen();
    // sock->setnonblocking();

    // std::function<void()> cb = std::bind(&Server::newConnection, this, sock);

    // Channel *ch = new Channel(sock->getFd(), loop);
    // ch->setCallback(cb);
    // ch->EnReadable();

    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor = new Acceptor(loop);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
}

void Server::handleEventET(int nfd)
{
    char buf[READ_SERVER_BUFFER];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t bytesize = read(nfd, buf, sizeof(buf));
        if (bytesize > 0)
        {
            printf("msg from client fd %d:%s\n", nfd, buf);
            write(nfd, buf, sizeof(buf));
        }
        else if (bytesize == 0)
        {
            printf("client fd %d disconnected\n", nfd);
            close(nfd);
            break;
        }
        else if (bytesize == -1 && errno == EINTR)
        {
            continue;
            // close(nfd);
            // errinfo(true, "socket read error");
        }
        else if (bytesize == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            break;
        }
    }
}

void Server::handleEventLT(int nfd)
{
    char buf[READ_SERVER_BUFFER];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t bytesize = read(nfd, buf, sizeof(buf));

        if (bytesize > 0)
        {
            printf("msg from client fd %d:%s\n", nfd, buf);
            write(nfd, buf, sizeof(buf));
        }
        else if (bytesize <= 0)
        {
            printf("client fd %d disconnected\n", nfd);
            close(nfd);
        }
    }
}

void Server::newConnection(Socket *sk)
{
    /*  SockAddress *addr = new SockAddress();
     int client_fd = sk->accept(addr);

     Socket *client_sc = new Socket(client_fd);
     client_sc->setnonblocking();
     Channel *ch = new Channel(client_fd, loop);
     std::function<void()> cb = std::bind(&Server::handleEventET, this, client_fd);
     ch->setCallback(cb);
     ch->EnReadable();
     printf("client IP: %s PORT:%d fd:%d  connected with server \n",
            inet_ntoa(addr->addr.sin_addr), ntohs(addr->addr.sin_port), client_fd); */

    Connection *newCon = new Connection(loop, sk);
    std::function<void(Socket *)> cb2 = std::bind(&Server::delConnection, this, std::placeholders::_1);
    newCon->setDelCallback(cb2);
    mapConnection[sk->getFd()] = newCon;
}

void Server::delConnection(Socket *sock)
{
    Connection *con = mapConnection[sock->getFd()];
    mapConnection.erase(sock->getFd());
    delete con;
}
