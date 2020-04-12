#pragma once

#include <net/Rtp.hpp>
#include <net/UdpSocket.hpp>
#include <buf/BufferSegment.hpp>

namespace mux {
namespace net {

struct RtpData;

class RtpSession
{
public:
    RtpSession(int16_t port);
    RtpSession(std::string bindIpV4, int16_t port);
    RtpSession(const net::SockaddrIn& address);

public:
    bool getData(RtpData& outData);

public:
    bool isClosed() const;

private:
    UdpSocket mSocket;
    bool mIsClosed;
};


struct RtpData
{
    Rtp rtp;
    size_t payloadOffset;
    buf::BufferSegment data;
};

}} //namespace mux::net