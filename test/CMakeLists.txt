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

set(test-source
        DummyTest.cpp)

# Build & link tests
add_executable(${PROJECT_TEST_TARGET} AllTests.cpp ${test-source})
target_link_libraries(${PROJECT_TEST_TARGET} gtest_main)

# C Test
add_test(NAME cpputest-runner COMMAND ${PROJECT_TEST_TARGET})

# Coverage Report
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

    include(CodeCoverage)
    set(COVERAGE_GCOVR_EXCLUDES '${PROJECT_ROOT}/test' '${PROJECT_ROOT}/vendor')
    SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(NAME xml_coverage EXECUTABLE ${PROJECT_TEST_TARGET} DEPENDENCIES ${PROJECT_TEST_TARGET})
    SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage EXECUTABLE ${PROJECT_TEST_TARGET} DEPENDENCIES l${PROJECT_TEST_TARGET})
endif()
