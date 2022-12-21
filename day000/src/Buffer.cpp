#include "Buffer.h"

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
    clear();
}

void Buffer::append(const char *str, int strsize)
{
    for (size_t i = 0; i < strsize; i++)
    {
        if (str[i]=='\0')break;
        buf.push_back(str[i]);
    }
}

void Buffer::clear()
{
    buf.clear();
}

const char *Buffer::getchar()
{
    return buf.c_str();
}

ssize_t Buffer::size()
{
    return buf.size();
}
