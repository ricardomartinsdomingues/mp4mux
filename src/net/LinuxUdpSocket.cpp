#include "net/UdpSocket.hpp"

#include <buf/BufferSegment.hpp>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define INVALID_FD -1

using namespace mux::net;

namespace {

SockaddrIn createSockaddr(const std::string& bindIpV4, uint16_t port)
{
    struct SockaddrIn servaddr;
    servaddr.family = AF_INET;
    servaddr.port = htons(port);

   if(inet_pton(servaddr.family , bindIpV4.c_str(), &servaddr.addr) < 1)
   {
       throw InvalidIpFormat(bindIpV4, "Invalid Ip format");
   }

   return servaddr;
}

} //namespace <anonymous>



UdpSocket::UdpSocket(uint16_t port)
    : UdpSocket("0.0.0.0", port)
{ }

UdpSocket::UdpSocket(const std::string& bindIpV4, uint16_t port)
    : UdpSocket(createSockaddr(bindIpV4, port))
{ }

UdpSocket::UdpSocket(const SockaddrIn& servaddr)
    : mSocketFD(INVALID_FD)
{
    mAddr = servaddr;

    if ( (mSocketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        throw UdpSocketError("socket creation failed"); 
    } 
 
    // Bind the socket with the server address 
    if ( bind(mSocketFD, (const struct sockaddr *)&mAddr,
            sizeof(mAddr)) < 0 ) 
    { 
        close(mSocketFD);
        mSocketFD = INVALID_FD;
        throw UdpSocketError("socket bind failed");
    }
}

UdpSocket::~UdpSocket()
{
    if(mSocketFD != INVALID_FD)
    {
        close(mSocketFD);
        mSocketFD = INVALID_FD;
    }
}

UdpSocket& UdpSocket::move(UdpSocket&& rhs)
{
    mAddr = std::move(rhs.mAddr);
    mSocketFD = std::exchange(rhs.mSocketFD, INVALID_FD);
    return *this;
}

int64_t UdpSocket::receiveFrom(buf::BufferSegment& buffer, SockaddrIn& addr)
{
    unsigned addrLen = sizeof(SockaddrIn);
    return recvfrom(mSocketFD, static_cast<char *>(buffer), buffer.size(),
                MSG_WAITALL, reinterpret_cast< struct sockaddr *>(&addr),
                &addrLen); 
}

int64_t UdpSocket::sendTo(buf::BufferSegment& buffer, SockaddrIn& addr)
{
    return sendto(mSocketFD, static_cast<const char *>(buffer), buffer.size(),
        MSG_CONFIRM, reinterpret_cast< struct sockaddr *>(&addr), 
            sizeof(SockaddrIn)); 
}
