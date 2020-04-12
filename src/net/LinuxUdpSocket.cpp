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

using namespace net;

UdpSocket::UdpSocket(uint16_t port)
    : UdpSocket("0.0.0.0", port)
{ }

UdpSocket::UdpSocket(const std::string& ip, uint16_t port)
    : mSocketFD(INVALID_FD)
{

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port); 

    if(inet_pton(servaddr.sin_family, ip.c_str(), &servaddr.sin_addr) < 1)
    {
        throw InvalidIpFormat(ip, "Invalid Ip format");
    }

    if ( (mSocketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        throw UdpSocketError("socket creation failed"); 
    } 
 
    // Bind the socket with the server address 
    if ( bind(mSocketFD, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        close(mSocketFD);
        throw UdpSocketError("socket bind failed");
    } 

}

UdpSocket::~UdpSocket()
{
    close(mSocketFD);
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
