cmake_minimum_required(VERSION 3.10)
project(SmartParking LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(smart_parking main.cpp ${SOURCES})
target_include_directories(smart_parking PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp")
    enable_testing()
    add_executable(run_tests tests/test_main.cpp ${SOURCES})
    target_include_directories(run_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    add_test(NAME UnitTests COMMAND run_tests)
endif()
