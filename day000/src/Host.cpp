#include "Host.h"
#include "Socket.h"
#include "SockAddress.h"
#include "Epoll.h"
#include "util.h"
#include "Channel.h"

#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define WRITE_SERVER_BUFFER 1024
#define READ_SERVER_BUFFER 1024
Host::Host(/* args */) : bETMode(true)
{
}

Host::~Host()
{
}

int Host::DoLT(Socket *sk, Epoll *ep, epoll_event *events, int nfds)
{
    for (size_t i = 0; i < nfds; i++)
    {
        int nfd = events[i].data.fd;
        if (nfd == sk->getFd())
        {
            SockAddress *addr = new SockAddress();
            int client_fd = sk->accept(addr);
            ep->addEpFd(client_fd, false);
            printf("client IP: %s PORT:%d fd:%d  connected with server \n",
                   inet_ntoa(addr->addr.sin_addr), ntohs(addr->addr.sin_port), client_fd);
        }
        else if (events[i].events & EPOLLIN)
        {
            char buf[READ_SERVER_BUFFER];
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
    return 0;
}

int Host::DoET2(Socket *sk, Epoll *ep, std::vector<Channel *> vCh)
{
    if (!sk || !ep)
    {
        return -1;
        /* code */
    }

    for (size_t i = 0; i < vCh.size(); i++)
    {
        /* code */
        Channel *pCh = vCh[i];
        int nfd = pCh->getSockFd();
        if (nfd == sk->getFd())
        {
            SockAddress *addr = new SockAddress();
            int client_fd = sk->accept(addr);
            Socket *client_sc = new Socket(client_fd);
            client_sc->setnonblocking();
            Channel *ch = new Channel(client_fd, ep);
            ch->EnReadable();
            printf("client IP: %s PORT:%d fd:%d  connected with server \n",
                   inet_ntoa(addr->addr.sin_addr), ntohs(addr->addr.sin_port), client_fd);
        }
        else if (pCh->getEvent() & EPOLLIN)
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
        else
        {
        }
    }

    return 0;
}

int Host::DoLT2(Socket *sk, Epoll *ep, std::vector<Channel *> vCh)
{
    if (!sk || !ep)
    {
        return -1;
        /* code */
    }

    for (size_t i = 0; i < vCh.size(); i++)
    {
        Channel *pch = vCh[i];
        int nfd = pch->getSockFd();
        if (nfd == sk->getFd())
        {
            SockAddress *addr = new SockAddress();
            int client_fd = sk->accept(addr);
            Socket *client_sc = new Socket(client_fd);
            client_sc->setnonblocking();
            Channel *client_ch = new Channel(client_fd, ep);
            // ep->addEpFd(client_fd,false);
            printf("client IP: %s PORT:%d fd:%d  connected with server \n",
                   inet_ntoa(addr->addr.sin_addr), ntohs(addr->addr.sin_port), client_fd);
        }
        else if (pch->getEvent() & EPOLLIN)
        {
            char buf[READ_SERVER_BUFFER];
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
    return 0;
}

int Host::DoET(Socket *sk, Epoll *ep, epoll_event *events, int nfds)
{
    if (nfds < 1)
    {
        return -1;
        /* code */
    }

    for (size_t i = 0; i < nfds; i++)
    {
        int nfd = events[i].data.fd;
        if (nfd == sk->getFd())
        {
            SockAddress *addr = new SockAddress();
            int client_fd = sk->accept(addr);
            ep->addEpFd(client_fd, false);
            printf("client IP: %s PORT:%d fd:%d  connected with server \n",
                   inet_ntoa(addr->addr.sin_addr), ntohs(addr->addr.sin_port), client_fd);
        }
        else if (events[i].events & EPOLLIN)
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
        else
        {
        }
    }
    return 0;
}

int Host::StartClient()
{
    Socket *sock = new Socket();
    SockAddress *addr = new SockAddress();
    printf("use %d\n", sock->getFd());
    sock->connect(addr);
    while (true)
    {
        char buff[WRITE_SERVER_BUFFER];
        bzero(buff, sizeof(buff));

        scanf("%s", buff);
        ssize_t writebyte = sock->write(buff, sizeof(buff));
        if (writebyte == -1)
        {
            printf("server is disconnect stop wirte msg");
            break;
        }
        bzero(buff, sizeof(buff));
        ssize_t readbyte = sock->read(buff, sizeof(buff));
        if (readbyte > 0)
        {
            printf("received server msg:%s\n", buff);
            // continue;
        }
        else if (readbyte == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }
        else if (readbyte == -1)
        {
            sock->close();
            errinfo(true, "socket read error");
        }
    }
    sock->close();
    delete sock;
    return 0;
}

int Host::StartServer()
{

    Socket *sock = new Socket();
    SockAddress *addr = new SockAddress();
    sock->setsockop();
    sock->bind(addr);
    sock->listen();
    Epoll *ep = new Epoll();
    ep->addEpFd(sock->getFd(), bETMode);

    struct epoll_event events[1024];
    bzero(&events, sizeof(events));
    while (1)
    {
        int nSize = ep->getEvents(events);
        if (bETMode)
        {
            DoET(sock, ep, events, nSize);
        }
        else
        {
            DoLT(sock, ep, events, nSize);
        }
    }
    delete sock;
    delete addr;
    delete ep;
    return 0;
}

int Host::StartServer2()
{
    Socket *sock = new Socket();
    SockAddress *addr = new SockAddress();
    sock->setsockop();
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    Epoll *ep = new Epoll();
    Channel *ch = new Channel(sock->getFd(), ep);
    ch->EnReadable();
    while (1)
    {
        std::vector<Channel *> vCh = ep->poll();
        if (bETMode)
        {
            DoET2(sock, ep, vCh);
        }
        else
        {
            DoLT2(sock, ep, vCh);
        }
    }
    delete sock;
    delete addr;
    delete ep;
    return 0;
}