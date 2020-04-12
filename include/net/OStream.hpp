#pragma once

#include <net/Rtp.hpp>
#include <ostream>

namespace mux {
namespace net {

 struct RtpDump
 {
     RtpDump(const Rtp& rtp)
        : mRtp(rtp)
    { }

    friend std::ostream& operator<< (std::ostream& os, const RtpDump& rtpDump)
    {
        return os 
            << "RTP Header "
            << "|v="    << rtpDump.mRtp.getVersion()
            << "|p="    << rtpDump.mRtp.hasPadding()
            << "|cc="   << rtpDump.mRtp.getCsrcCount()
            << "|m="    << rtpDump.mRtp.hasMarker()
            << "|pt="   << rtpDump.mRtp.getPayloadType()
            << "|seq="  << rtpDump.mRtp.getSequence()
            << "|ts="   << rtpDump.mRtp.getTimestamp()
            << "|ssrc=" << rtpDump.mRtp.getSyncSrc();
    }

    const Rtp& mRtp;
 };

}} //namespace mux::net