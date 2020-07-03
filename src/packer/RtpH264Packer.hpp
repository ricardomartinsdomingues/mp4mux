#pragma once

#include <packer/RtpPacker.hpp>

namespace mux {

class RtpH264Packer final
    : public RtpPacker
{

public:
    RtpH264Packer(const PostFrameCb& postFrameCb);
    void decodeRtpPack(const net::RtpData& rtpInfo, const buf::BufferSegment& rtpData) override;

private:
    void decodeStap(const net::RtpData& rtpInfo, const buf::BufferSegment& rtpData);
    void decodeFu(const net::RtpData& rtpInfo, const buf::BufferSegment& rtpData);

};

} //namespace mux