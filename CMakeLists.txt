cmake_minimum_required(VERSION 3.10)
project(atari-vcs-emu)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

add_executable(atari-vcs-emu
        src/main.cpp
        src/Cart.cpp
        src/AtariVCS.cpp
        src/MOS6502Core.cpp
        src/TIACore.cpp
        src/HexDump.cpp
        src/Memory.cpp
        src/RIOTCore.cpp
        src/Opcodes.cpp)

target_compile_definitions(atari-vcs-emu PUBLIC DEBUG=1)

# subproject for unit tests
add_subdirectory(vendor/googletest)
add_subdirectory(test atari-2600-tests)
