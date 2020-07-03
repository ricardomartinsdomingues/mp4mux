#include "packer/RtpPackerFactory.hpp"
#include "RtpH264Packer.hpp"

using namespace mux;

std::unique_ptr< RtpPacker > RtpPackerFactory::createForRtpH264(const RtpPacker::PostFrameCb& postFrameCb)
{
    return std::make_unique< RtpH264Packer >(postFrameCb);
}