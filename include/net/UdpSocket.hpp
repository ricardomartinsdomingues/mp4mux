#pragma once

#include <net/SocketTypes.hpp>

#include <string>
#include <stdexcept>
#include <utility>

namespace buf { class BufferSegment; }

namespace net
{

class UdpSocket
{

public:
    UdpSocket(uint16_t port);
    UdpSocket(const std::string& ip, uint16_t port);
    UdpSocket(const SockaddrIn& addr);
    ~UdpSocket();

    int64_t receiveFrom(buf::BufferSegment& buffer, SockaddrIn& addr);
    int64_t sendTo(buf::BufferSegment& buffer, SockaddrIn& addr);

private:
    int mSocketFD;
    SockaddrIn mAddr;
};


class UdpSocketError : std::runtime_error
{
public:
    template< class T >
    UdpSocketError(T&& msg)
        : std::runtime_error(std::forward< T >(msg)) { }
};

class InvalidIpFormat : std::invalid_argument
{
public:
    template < class T, class K >
    InvalidIpFormat(T&& invalidIp, K&& message)
        : std::invalid_argument(std::forward < K >(message))
        , mInvalidIp(std::forward < T >(invalidIp))
    { }

    const std::string& getInvalidIpValue() { return mInvalidIp; }

private:
    std::string mInvalidIp;

};

} // namespace net