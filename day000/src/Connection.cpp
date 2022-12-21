#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SockAddress.h"
#include "Channel.h"
#include "Buffer.h"

#include <functional>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#define WRITE_SERVER_BUFFER 1024
#define READ_SERVER_BUFFER 1024
Connection::Connection(EventLoop *lp, Socket *sk) : loop(lp), sock(sk)
{
   
    Channel *ch = new Channel(sk->getFd(), loop);
    std::function<void()> cb = std::bind(&Connection::echoET, this, sk->getFd());
    channel = new Channel(sock->getFd(), loop);
    channel->setCallback(cb);
    //channel->setCallback(std::bind(&Connection::echoET, this, sk->getFd()));
    channel->EnReadable();
}

Connection::~Connection()
{
}

void Connection::echoET(int nfd)
{
    Buffer buffer;
    char buf[READ_SERVER_BUFFER];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t bytesize = read(nfd, buf, sizeof(buf));
        if (bytesize > 0)
        {
            /* printf("msg from client fd %d:%s\n", nfd, buf);
            write(nfd, buf, sizeof(buf)); */
            buffer.append(buf,sizeof(buf));
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
            printf("msg from client fd %d:[size%i]%s\n", nfd,buffer.size(), buffer.getchar());
            write(nfd, buffer.getchar(), buffer.size());
            buffer.clear();
            break;
        }
    }
}

void Connection::echoLT(int nfd)
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

void Connection::setDelCallback(del_Callback dk)
{
    delCallback = dk;
}
