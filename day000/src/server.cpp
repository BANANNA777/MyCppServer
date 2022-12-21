// #pragma once
// #include "Host.h"
#include "Server.h"
#include "EventLoop.h"
#include <stdio.h>
int main()
{
    // Host host;
    // host.StartServer2();
    EventLoop *loop = new EventLoop();
    Server *serv = new Server(loop);
    loop->loop();
    return 0;
}