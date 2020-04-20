cmake_minimum_required(VERSION 3.2.2)
project(mp4muxer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT MSVC)
    add_compile_options(-pthread)
    add_compile_options(-Werror)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

set(buf_src
    src/buf/Buffer.cpp
    src/buf/BufferSegment.cpp
)

set(net_src
    src/net/LinuxUdpSocket.cpp
    src/net/Sock.cpp
    src/net/Rtp.cpp
    src/net/RtpSession.cpp
)

set(all_src
    ${buf_src}
    ${net_src}
)

add_executable(mp4mux
    src/main.cpp
    ${all_src}
)

add_executable(mp4mux_test
    src/main_test.cpp
    ${all_src}
)

target_link_libraries(mp4mux -lpthread)
target_link_libraries(mp4mux_test -lpthread)
