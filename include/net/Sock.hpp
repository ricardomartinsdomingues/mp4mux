#pragma once

#include <cstdint>

namespace mux {
namespace net {

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

uint32_t mux_htonl(uint32_t hostlong);
uint16_t mux_htons(uint16_t hostshort);
uint32_t mux_ntohl(uint32_t netlong);
uint16_t mux_ntohs(uint16_t netshort);

}}