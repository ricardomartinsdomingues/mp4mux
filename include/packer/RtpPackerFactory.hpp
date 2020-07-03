#pragma once

#include <packer/RtpPacker.hpp>
#include <memory>

namespace mux {

class RtpPackerFactory
{
public:
    static std::unique_ptr< RtpPacker > createForRtpH264(const RtpPacker::PostFrameCb& postFrameCb);
};

} //namespace mux