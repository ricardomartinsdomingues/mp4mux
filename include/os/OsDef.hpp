#pragma once

#if defined(_WIN32) || defined(WIN32)
    #include <Windows.h>

    #define OS_UNIX 0
    #define OS_LINUX 0
    #define OS_Windows 1

    #if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
        #define MUX_LITTLE_ENDIAN 1
        #define MUX_BIG_ENDIAN 0
    #elif REG_DWORD == REG_DWORD_BIG_ENDIAN
        #define MUX_LITTLE_ENDIAN 0
        #define MUX_BIG_ENDIAN 1
    #else
        #error "Endianness not supported"
    #endif

#elif defined(__GNUC__) || defined(__GNUG__)

    #define OS_UNIX __unix__
    #define OS_LINUX __linux__
    #define OS_Windows 0

    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define MUX_LITTLE_ENDIAN 1
        #define MUX_BIG_ENDIAN 0
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define MUX_LITTLE_ENDIAN 0
        #define MUX_BIG_ENDIAN 1
    #else
        #error "Endianness not supported"
    #endif
#else
    #error "Target not supported"
#endif
