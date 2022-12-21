#include "SockAddress.h"
#include <strings.h>
SockAddress::SockAddress(/* args */)
{
    bzero(&addr, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    len = sizeof(addr);
}

SockAddress::~SockAddress()
{
}