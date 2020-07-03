#pragma once

#include <net/RtpSession.hpp>
#include <functional>

namespace mux {

class RtpPacker
{
public:
    using PostFrameCb = std::function< void(const net::Rtp&, const buf::BufferSegment& frameData) >;

protected:
    RtpPacker(const PostFrameCb& posFrameCb)
        : mPostFrameCb(posFrameCb)
    { }

public:
    virtual ~RtpPacker() = default;
    virtual void decodeRtpPack(const net::RtpData& rtpInfo, const buf::BufferSegment& rtpData) = 0;

protected:
    const PostFrameCb mPostFrameCb;
};

} //namespace mux
