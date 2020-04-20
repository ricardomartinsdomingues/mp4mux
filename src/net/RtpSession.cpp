#include "net/RtpSession.hpp"
#include <iostream>

using namespace mux::net;
using namespace mux::buf;

RtpSession::RtpSession(int16_t port)
    : mSocket(port)
    , mIsClosed(false)
{ }

RtpSession::RtpSession(std::string bindIp, int16_t port)
    : mSocket(bindIp, port)
    , mIsClosed(false)
{ }

RtpSession::RtpSession(const net::SockaddrIn& address)
    : mSocket(address)
    , mIsClosed(false)
{ }

bool RtpSession::getData(RtpData& outData, buf::BufferSegment buff)
{
    if(mIsClosed)
    {
        std::cerr << "Can't read from closed socket" << std::endl;
        return false;
    }

    net::SockaddrIn addr;
    int64_t bytesReceived =  mSocket.receiveFrom(buff, addr);
    
    if(bytesReceived == 0)
    {
        std::cout << "Socket close" << std::endl;
        mIsClosed = true;
        return false;
    }

    if(bytesReceived < 0)
    {
        std::cout << "Error to read from socket" << std::endl;
        return false;
    }

    outData.rtp = Rtp::decode(buff);
    if(outData.rtp.getVersion() != Rtp::kVersion)
    {
        std::cout << "Unsupported RTP version. Version: " << outData.rtp.getVersion() << std::endl;
        return false;
    }

    outData.payloadOffset = Rtp::kRTPHeaderMinSize + (outData.rtp.getCsrcCount() * 4);
    outData.paddingSize = 0; //TODO: reading padding. For now we assume it is zero
    outData.payloadSize = static_cast< size_t >(bytesReceived) - outData.payloadOffset - outData.paddingSize;
    return true;
}