#pragma once

#include <vector>

namespace mux {
namespace buf {

class Buffer

{
public:
    using TIter = std::vector< char >::iterator;
    using TConstIter = std::vector< char >::const_iterator;

public:
    Buffer(size_t size);

public:
    void resize(size_t newSize);
    void increase(size_t size);
    void reduce(size_t size);

public:
    char* operator[](size_t pos)
    {
        return &mBuff[pos];
    }

    const char* operator[](size_t pos) const
    {
        return &mBuff[pos];
    }

    explicit operator char*() noexcept { return mBuff.data(); }

    explicit operator const char*() const noexcept { return mBuff.data(); }

    TIter begin() { return mBuff.begin(); }
    TConstIter begin() const { return mBuff.begin(); }

    TIter end() { return mBuff.begin() + mBuff.capacity(); }
    TConstIter end() const { return mBuff.begin() + mBuff.capacity(); }

    size_t size() const { return mBuff.size(); }

private:
    std::vector< char > mBuff;
};

}}