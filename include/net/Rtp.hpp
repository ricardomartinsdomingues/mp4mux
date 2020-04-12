
#pragma once

#include <buf/BufferSegment.hpp>
#include <os/OsDef.hpp>
#include <net/Sock.hpp>

namespace mux {
namespace net {

#pragma pack(1)
struct RTPHeader
{
#if defined(MUX_BIG_ENDIAN) && (MUX_BIG_ENDIAN == 1)
    uint16_t version : 2;
    uint16_t padding : 1;
    uint16_t extention : 1;
    uint16_t cc : 4;
    uint16_t marker : 1;
    uint16_t payloadType : 7;
#else
    uint16_t cc:4;
    uint16_t extention:1;
    uint16_t padding:1;
    uint16_t version:2;
    uint16_t payloadType:7;
    uint16_t marker:1;
#endif
    uint16_t sequence : 16;
    uint32_t timestamp : 32;
    uint32_t ssrc : 32;
};
#pragma pack()


class Rtp 
{
public:
    Rtp() { }

public:
    static Rtp decode(const char* buff, size_t count);
    static Rtp decode(const buf::BufferSegment& buffSegment);
    static Rtp decode(const buf::Buffer& buff, size_t count, size_t offset = 0);

public:
    uint16_t getVersion() const { return mRtpheader.version; }
    bool hasPadding() const { return mRtpheader.padding; }
    bool hasExtention() const { return mRtpheader.extention; }
    uint16_t getCsrcCount() const { return mRtpheader.cc; }
    bool hasMarker() const { return mRtpheader.marker; }
    uint16_t getPayloadType() const { return mRtpheader.payloadType; }

    // The follwing methods need to be converted to right endianness
    uint16_t getSequence() const { return ntohs(mRtpheader.sequence); }
    uint32_t getTimestamp() const { return net::ntohl(mRtpheader.timestamp); }
    uint32_t getSyncSrc() const { return net::ntohl(mRtpheader.ssrc); }


private:
    RTPHeader mRtpheader;

public:
    constexpr static size_t kRTPHeaderMinSize = sizeof(RTPHeader);
};

}} // namespace mux::net