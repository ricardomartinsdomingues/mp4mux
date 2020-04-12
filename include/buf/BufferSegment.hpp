#pragma once

#include <buf/Buffer.hpp>

namespace buf
{

class BufferSegment
{

public:
    BufferSegment(Buffer &buff);
    BufferSegment(Buffer &buff, size_t offset, size_t count);

public:
    char* operator[](size_t pos)
    {
        return mBuff[pos + mOffset];
    }

    const char* operator[](size_t pos) const
    {
        return mBuff[pos + mOffset];
    }

    operator char*() { return mBuff[mOffset]; }

    operator const char*() const { return mBuff[mOffset]; }

    size_t size() const { return mCount; }

    size_t offset() const { return mOffset; }

private:
    Buffer &mBuff;
    size_t mOffset;
    size_t mCount;
};

}