#include "RtpH264Packer.hpp"
#include <net/Sock.hpp>
#include <iostream>

using namespace mux;

namespace {

static const uint8_t kNaluTypeStapA =    24;
static const uint8_t kNaluTypeStapB =    25;
static const uint8_t kNaluTypeMtap16 =   26;
static const uint8_t kNaluTypeMtap32 =   27;
static const uint8_t kNaluTypeFuA =      28;
static const uint8_t kNaluTypeFuB =      29;

// 5.3. NAL Unit Octet Usage
 /*
 +-+-+-+-+-+-+-+-+
 |0|1|2|3|4|5|6|7|
 +-+-+-+-+-+-+-+-+
 |F|NRI|  Type   |
 +-+-+-+-+-+-+-+-+
 */

#pragma pack(1)
struct Nalu {
#if defined(MUX_BIG_ENDIAN) && (MUX_BIG_ENDIAN == 1)
    uint8_t forbidden_zero_bit: 1;
    uint8_t nri: 2;
    uint8_t type: 5;
#else
    uint8_t type: 5;
    uint8_t nri: 2;
    uint8_t forbidden_zero_bit: 1;
#endif
};

#pragma pack()

uint16_t read16(buf::BufferSegment::TConstIter& it)
{
    const uint16_t res = net::mux_ntohs(reinterpret_cast< const uint16_t& >(*it));
    it+=2;
    return res;
}

} //namespace <anonymous>

// RFC: https://tools.ietf.org/html/rfc3984

RtpH264Packer::RtpH264Packer(const PostFrameCb& postFrameCb)
    : RtpPacker(postFrameCb)
{ }

void RtpH264Packer::decodeRtpPack(const net::RtpData& rtpInfo, const buf::BufferSegment& rtpData)
{
    const Nalu *nalu = reinterpret_cast< const Nalu * >(rtpData[rtpInfo.payloadOffset]);

    switch (nalu->type)
    {
    case kNaluTypeStapA:
    case kNaluTypeStapB: 
        decodeStap(rtpInfo, rtpData);
        break;
    case kNaluTypeMtap16: throw "Mtap16 is not supported yet";
    case kNaluTypeMtap32: throw "Mtap32 is not supported yet";
    case kNaluTypeFuA: throw "FuA is not supported yet";
    case kNaluTypeFuB: throw "FuB is not supported yet";
    default:
        if(nalu->type > 0 && nalu->type < 24)
        {
            mPostFrameCb(rtpInfo.rtp
            , buf::BufferSegment(rtpData, rtpInfo.payloadOffset + sizeof(Nalu), rtpInfo.payloadSize -sizeof(Nalu))
            );
        }
        else
        {
            throw "Invalid NALU type";
        }
    }
}


// 5.7.1. Single-Time Aggregation Packet (STAP)
/*
                     1                   2                     3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           RTP Header                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|STAP-B NAL HDR |            DON                |  NALU 1 Size  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| NALU 1 Size   | NALU 1 HDR    |         NALU 1 Data           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
:                                                               :
+               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|               | NALU 2 Size                   |   NALU 2 HDR  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                            NALU 2 Data                        |
:                                                               :
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :    ...OPTIONAL RTP padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void RtpH264Packer::decodeStap(const net::RtpData& rtpInfo, const buf::BufferSegment& rtpData)
{

    const Nalu *nalu = reinterpret_cast< const Nalu * >(rtpData[rtpInfo.payloadOffset]);
    
    if(!(nalu->type == kNaluTypeStapA || nalu->type == kNaluTypeStapB))
    {
        throw "Incorrect NAL"; //TODO: Replace for a properly exception type
    }
    
    // StapB add 2 additional bytes for DOM
    const size_t firstNailOffset = sizeof(Nalu) + (nalu->type == kNaluTypeStapB ? 2 : 0 );

    //Do I need to know the DOM value? For now I can't see the reason
    //uint16_t dom = readDom;

    auto end = rtpData.begin() + firstNailOffset + rtpInfo.payloadSize + 1;

    if(end > rtpData.end())
    {
        throw "Wrong buffer size."; //TODO. Replace for a properly exception type
    }

    size_t bytesToRead = rtpInfo.payloadSize - firstNailOffset;
    auto currIt= rtpData.begin() + firstNailOffset;
    while(currIt < end)
    {
        auto endNal = currIt + read16(currIt) + 1;
        if(endNal > end)
        {
            // Maybe packet lost.
            throw "Packet lost"; //TODO. Replace for a properly exception type
        }

        mPostFrameCb(rtpInfo.rtp, buf::BufferSegment(rtpData, currIt, endNal - 1));
        currIt = endNal;
    }
}

// 5.8. Fragmentation Units (FUs)
/*
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  FU indicator |   FU header   |              DON              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-|
|                                                               |
|                          FU payload                           |
|                                                               |
|                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                               :   ...OPTIONAL RTP padding     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void RtpH264Packer::decodeFu(const net::RtpData& rtpInfo, const buf::BufferSegment& rtpData)
{

}

