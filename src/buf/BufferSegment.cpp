#include <buf/BufferSegment.hpp>

using namespace mux::buf;

BufferSegment::BufferSegment(Buffer &buff)
    : BufferSegment(buff, 0 , buff.size())
{ }

BufferSegment::BufferSegment(Buffer &buff, size_t offset, size_t count)
    : mBuff(buff)
    , mOffset(offset)
    , mCount(count)
{ }

BufferSegment::BufferSegment(const BufferSegment& buffSegment, size_t offset, size_t count)
    : BufferSegment(buffSegment.mBuff, buffSegment.mOffset + offset, count)
{ }

BufferSegment::BufferSegment(Buffer &buff, const TConstIter& first, const TConstIter& last)
    : mBuff(buff)
{
    const auto begin = buff.begin();

    if(first < begin || last >= buff.end())
    {
        throw "Invalid iterators"; //TODO: Find a properly exception type
    }

    mOffset = first - begin;
    mCount = last - first;
}

BufferSegment::BufferSegment(const BufferSegment& buffSegment, const TConstIter& first, const TConstIter& last)
    : BufferSegment(buffSegment.mBuff, first, last)
{
    if(first < buffSegment.begin() || last >= buffSegment.end())
    {
        throw "Invalid iterators"; //TODO: Find a properly exception type
    }
}