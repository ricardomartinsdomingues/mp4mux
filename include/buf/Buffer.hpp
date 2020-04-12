#pragma once

#include <vector>

namespace mux {
namespace buf {

class Buffer
{

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

    explicit operator char*() { return &mBuff.front(); }

    explicit operator const char*() const { return &mBuff.front(); }

    size_t size() const { return mBuff.size(); }

private:
    std::vector< char > mBuff;
};

}}