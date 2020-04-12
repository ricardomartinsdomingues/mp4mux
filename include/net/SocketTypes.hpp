#pragma once

#include <stdint.h>

struct InAddr
{
    uint32_t    addr;
};

struct SockaddrIn
{
    int16_t         family;
    uint16_t        port;
    InAddr          addr;
    char            zero[8];
};
