#include "net/Sock.hpp"
#include <os/OsDef.hpp>

#if defined(OS_Windows) && (OS_Windows!=0)
    #include <Winsock.h>
#else
    #include <arpa/inet.h>
#endif

uint32_t mux::net::htonl(uint32_t hostlong)
{
    return ::htonl(hostlong);
}

uint16_t mux::net::htons(uint16_t hostshort)
{
    return ::htons(hostshort);
}

uint32_t mux::net::ntohl(uint32_t netlong)
{
    return ::ntohl(netlong);
}

uint16_t mux::net::ntohs(uint16_t netshort)
{
    return ::ntohs(netshort);
}