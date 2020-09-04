cmake_minimum_required(VERSION 3.5)
project(atari-2600-emu-test)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
set(PROJECT_TEST_TARGET atari-2600-emu-test)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_INSTALL_LIBDIR ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

enable_testing()

set(vcs-emu-source
        ${PROJECT_ROOT}/src/MOS6502Core.cpp
        ${PROJECT_ROOT}/src/Memory.cpp
        ${PROJECT_ROOT}/src/Opcodes.cpp
        ${PROJECT_ROOT}/src/VCS/RIOTCore.cpp
        ${PROJECT_ROOT}/src/VCS/TIACore.cpp
        ${PROJECT_ROOT}/src/VCS/VCSMemory.cpp
        ${PROJECT_ROOT}/src/Cart.cpp
        ${PROJECT_ROOT}/src/HexDump.cpp)

set(test-source
        OpcodeTest.cpp TIATest.cpp)

include_directories(${PROJECT_ROOT}/src)

# Build & link tests
add_executable(${PROJECT_TEST_TARGET} AllTests.cpp ${test-source} ${vcs-emu-source})
target_link_libraries(${PROJECT_TEST_TARGET} gtest_main)
target_compile_definitions(${PROJECT_TEST_TARGET} PUBLIC DISABLE_CPU_CYCLE_ACCURACY)

# C Test
add_test(NAME gtest-runner COMMAND ${PROJECT_TEST_TARGET})

# Coverage Report
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

    include(CodeCoverage)
    set(COVERAGE_GCOVR_EXCLUDES '${PROJECT_ROOT}/test' '${PROJECT_ROOT}/vendor')
    SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML_XML(NAME coverage EXECUTABLE ${PROJECT_TEST_TARGET} EXECUTABLE_ARGS '--gtest_output=xml' DEPENDENCIES ${PROJECT_TEST_TARGET})
endif()
