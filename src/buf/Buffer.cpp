#include <buf/Buffer.hpp>
#include <stdexcept>
#include <algorithm>

using namespace buf;

Buffer::Buffer(size_t size)
    : mBuff(size)
{  }

void Buffer::resize(size_t newSize)
{
    if(newSize >= size())
    {
        mBuff.reserve(newSize);
    }
    else
    {
        std::vector < char > newBuff(newSize);
        std::copy(mBuff.begin(), mBuff.begin() + newSize, newBuff.begin());
        mBuff = std::move(newBuff);
    }
}

void Buffer::increase(size_t s)
{
    resize(size() + s);
}

void Buffer::reduce(size_t s)
{
    if(s > size())
    {
        throw std::out_of_range("Value to reduce is greater than the actull buffer size");
    }

    resize(size() - s);
}