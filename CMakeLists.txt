cmake_minimum_required(VERSION 3.20)

project(
    stomp-engine
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(stomp-dsp
    src/dsp/Gain.cpp
)

target_include_directories(stomp-dsp
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(stomp-engine
    src/main.cpp
)

target_link_libraries(stomp-engine
    PRIVATE
        stomp-dsp
)

target_compile_options(stomp-dsp
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

target_compile_options(stomp-engine
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

enable_testing()

add_executable(gain-test
    tests/GainTest.cpp
)

target_link_libraries(gain-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME GainTest
    COMMAND gain-test
)
