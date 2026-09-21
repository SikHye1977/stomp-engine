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
    src/dsp/EffectChain.cpp
    src/dsp/Overdrive.cpp
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

add_executable(effect-chain-test
    tests/EffectChainTest.cpp
)

target_link_libraries(effect-chain-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME EffectChainTest
    COMMAND effect-chain-test
)

add_executable(bypass-test
    tests/BypassTest.cpp
)

target_link_libraries(bypass-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME BypassTest
    COMMAND bypass-test
)

add_executable(parameter-test
    tests/ParameterTest.cpp
)

target_link_libraries(parameter-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME ParameterTest
    COMMAND parameter-test
)

add_executable(effect-parameter-test
    tests/EffectParameterTest.cpp
)

target_link_libraries(effect-parameter-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME EffectParameterTest
    COMMAND effect-parameter-test
)

add_executable(overdrive-test
    tests/OverdriveTest.cpp
)

target_link_libraries(overdrive-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME OverdriveTest
    COMMAND overdrive-test
)

add_executable(effect-chain-control-test
    tests/EffectChainControlTest.cpp
)

target_link_libraries(effect-chain-control-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME EffectChainControlTest
    COMMAND effect-chain-control-test
)

add_executable(real-time-safety-test
    tests/RealTimeSafetyTest.cpp
)

target_link_libraries(real-time-safety-test
    PRIVATE
        stomp-dsp
)

add_test(
    NAME RealTimeSafetyTest
    COMMAND real-time-safety-test
)
