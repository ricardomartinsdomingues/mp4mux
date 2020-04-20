#pragma once

#include <buf/Buffer.hpp>

namespace mux {
namespace buf {

class BufferSegment
{
public:
    using TIter = Buffer::TIter;
    using TConstIter = Buffer::TConstIter;

public:
    BufferSegment(Buffer &buff);
    BufferSegment(Buffer &buff, size_t offset, size_t count);
    BufferSegment(const BufferSegment& buffSegment, size_t offset, size_t count);
    BufferSegment(Buffer &buff, const TConstIter& first, const TConstIter& last);
    BufferSegment(const BufferSegment& buffSegment, const TConstIter& first, const TConstIter& last);

public:
    char* operator[](size_t pos)
    {
        return mBuff[pos + mOffset];
    }

    const char* operator[](size_t pos) const
    {
        return mBuff[pos + mOffset];
    }

    explicit operator char*() { return mBuff[mOffset]; }

    explicit operator const char*() const { return mBuff[mOffset]; }

    TIter begin() { return mBuff.begin() + mOffset; }
    TConstIter begin() const { return mBuff.begin() + mOffset; }

    TIter end() { return begin() + mCount; }
    TConstIter end() const { return begin() + mCount; }

    size_t size() const { return mCount; }

    size_t offset() const { return mOffset; }

private:
    Buffer &mBuff;
    size_t mOffset;
    size_t mCount;
};

}}