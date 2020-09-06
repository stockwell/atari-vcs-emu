cmake_minimum_required(VERSION 3.5)
project(atari-vcs-emu)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/SDL-mirror)

add_executable(atari-vcs-emu
        platform/Linux-x86_64/main.cpp
        src/Cart.cpp
        src/MOS6502Core.cpp
        src/HexDump.cpp
        src/Memory.cpp
        src/Opcodes.cpp
        src/VCS/AtariVCS.cpp
        src/VCS/DrawCounterDecodes.cpp
        src/VCS/RIOTCore.cpp
        src/VCS/TIACore.cpp
        src/VCS/VCSMemory.cpp
        src/VCS/TIAPlayer.cpp
        src/VCS/TIAMissile.cpp
        src/VCS/TIABall.cpp
        src/VCS/TIAPlayfield.cpp)

target_include_directories(atari-vcs-emu PUBLIC src)
target_compile_definitions(atari-vcs-emu PUBLIC DEBUG=1)
target_link_libraries(atari-vcs-emu SDL2main SDL2-static)

# subproject for unit tests
add_subdirectory(vendor/googletest)
add_subdirectory(test atari-2600-tests)
