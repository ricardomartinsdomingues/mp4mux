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
    bool getData(RtpData& outData, buf::BufferSegment buff);

public:
    bool isClosed() const { return mIsClosed; }

private:
    UdpSocket mSocket;
    bool mIsClosed;
};


struct RtpData
{
    /*! RTP Header info */
    Rtp rtp;

    /*! RTP payload offset */
    size_t payloadOffset;

    /*! RTP Payload size (With no RTP header neither padding) */
    size_t payloadSize;

    /*! RTP padding size */
    size_t paddingSize;
};

}} //namespace mux::net