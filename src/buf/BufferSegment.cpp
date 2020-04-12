#include <buf/BufferSegment.hpp>

using namespace buf;

BufferSegment::BufferSegment(Buffer &buff)
    : BufferSegment(buff, 0 , buff.size())
{ }

BufferSegment::BufferSegment(Buffer &buff, size_t offset, size_t count)
    : mBuff(buff)
    , mOffset(offset)
    , mCount(count)
{ }