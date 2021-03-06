#pragma once

#include <net/Sock.hpp>

#include <string>
#include <stdexcept>
#include <utility>

namespace mux {

namespace buf { class BufferSegment; }


namespace net
{

class UdpSocket
{

public:
    UdpSocket(uint16_t port);
    UdpSocket(const std::string& ip, uint16_t port);
    UdpSocket(const SockaddrIn& servaddr);
    UdpSocket(const UdpSocket& rhs) = delete;
    UdpSocket(UdpSocket&& rhs) { move(std::forward<UdpSocket>(rhs)); }
    ~UdpSocket();

    UdpSocket& operator=(const UdpSocket& rhs) = delete;
    UdpSocket& operator=(UdpSocket&& rhs) { return move(std::forward<UdpSocket>(rhs)); }


    int64_t receiveFrom(buf::BufferSegment& buffer, SockaddrIn& addr);
    int64_t sendTo(buf::BufferSegment& buffer, SockaddrIn& addr);

private:
    UdpSocket& move(UdpSocket&& rhs);

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

}} // namespace mux::net