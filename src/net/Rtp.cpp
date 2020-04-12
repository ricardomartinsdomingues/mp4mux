#include "net/Rtp.hpp"
#include <cstring>

using namespace mux::net;
using namespace mux::buf;

Rtp Rtp::decode(const char* buff, size_t count)
{
    if(count < kRTPHeaderMinSize)
    {
        throw "ERROR"; //TODO: add error type
    }

    Rtp rtp;
    const RTPHeader* rtpheader = reinterpret_cast< const RTPHeader* >(buff);
    rtp.mRtpheader = *rtpheader;
    return rtp;
}

Rtp Rtp::decode(const BufferSegment& buffSegment)
{
    return decode(buffSegment[0], buffSegment.size());
}

Rtp Rtp::decode(const Buffer& buff, size_t count, size_t offset)
{
    return decode(buff[offset], count);
}