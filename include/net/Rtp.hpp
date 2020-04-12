#pragma once

struct Rtp {
    unsigned version : 2;
    bool hasPadding : 1;
    bool hasExtention : 1;
    unsigned cc : 4;
    bool marker : 1;
    unsigned payloadType : 7;
    unsigned short sequence : 16;
    const unsigned timestamp : 32;

    size_t sizeWithoutExtension()
    {
        return sizeof(Rtp) + (cc * 4);
    }
};