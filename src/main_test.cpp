
#include <net/UdpSocket.hpp>
#include <buf/Buffer.hpp>
#include <buf/BufferSegment.hpp>
#include <net/Rtp.hpp>
#include <net/OStream.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <os/OsDef.hpp>

using namespace std::chrono_literals;
using namespace mux;

#define UDP_PORT 5000


class TestUdpTask
{
public:
    TestUdpTask()
        : mBuffer(1500)
        , mUdpSocket(UDP_PORT)
    { }

    virtual ~TestUdpTask() = default;

    virtual void run() = 0;


protected:
    buf::Buffer mBuffer;
    net::UdpSocket mUdpSocket;
};

class TestMeasureUdpTrafficTask : TestUdpTask
{

public:
    void run() override
    {
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
            buf::BufferSegment segment(mBuffer);
            bytesReceived = mUdpSocket.receiveFrom(segment, inAddr);
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
    }
};

class TestRTPHeaderDumpTask : TestUdpTask
{
public:
    TestRTPHeaderDumpTask(int numberOfPacket)
        : mNumberOfPacket(numberOfPacket)
    { }

public:
    void run() override
    {
        net::SockaddrIn inAddr;
        int64_t bytesReceived = 0;

        std::cout << "Udp socket listing" << std::endl;

        for(int i = 0; i < mNumberOfPacket || mNumberOfPacket == -1; i++)
        {
            buf::BufferSegment segment(mBuffer);
            bytesReceived = mUdpSocket.receiveFrom(segment, inAddr);
            if(bytesReceived < 0)
            {
                std::cout << "Udp socket closed" << std::endl;
                break;
            }

            if(bytesReceived < net::Rtp::kRTPHeaderMinSize)
            {
                std::cerr << "Receive packet lower than size of RTP header. Size received: " << bytesReceived;
                continue;
            }
            
            const net::Rtp rtp = net::Rtp::decode(mBuffer, bytesReceived);

            std::cout << net::RtpDump(rtp) << std::endl;
        }
    }

private:
    int mNumberOfPacket;
};

int main()
{
    //TestMeasureUdpTrafficTask().run();
    TestRTPHeaderDumpTask(50).run();

    return 0;
}