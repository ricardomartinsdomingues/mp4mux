
#include <net/UdpSocket.hpp>
#include <buf/Buffer.hpp>
#include <buf/BufferSegment.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <string>


using namespace std::chrono_literals;
using namespace mux;

int main()
{
    buf::Buffer buffer(1500);
    net::UdpSocket udpSocket(5000);
    net::SockaddrIn inAddr;
    int64_t bytesReceived = 0;
    int64_t bytesSecondCount = 0;
    std::string receiveBytesWritten = "0 B/s";
    int lastSecondProcessed = 0;
    auto start = std::chrono::steady_clock::now();

    uint32_t bytesCount = 0;

    std::cout << "Receiving: " << receiveBytesWritten << std::flush;

    while (true)
    {
        buf::BufferSegment segment(buffer);
        bytesReceived = udpSocket.receiveFrom(segment, inAddr);
        if(bytesReceived < 0)
        {
            break;
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = end - start;
        int second = (int)elapsedSeconds.count();
        if(second > lastSecondProcessed)
        {
            std::string strToWrite("");
            if((second - lastSecondProcessed) > 1)
            {
                strToWrite = "0 B/s";
            }
            else
            {
                strToWrite = std::to_string((double)bytesSecondCount/(double)1024) + " KB/s"; 
            }

            std::cout << std::string(receiveBytesWritten.size(), '\b')  << std::flush;
            std::cout << strToWrite << std::flush;

            receiveBytesWritten = std::move(strToWrite);
            lastSecondProcessed = second;
            bytesSecondCount = 0;
        }

        bytesSecondCount+=bytesReceived;
    }
    
    return 0;
}