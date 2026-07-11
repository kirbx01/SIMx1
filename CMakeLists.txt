cmake_minimum_required(VERSION 3.10)
project(SIMx1_sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SIMx1_core src/gate.cpp src/wire.cpp src/circuit.cpp)
target_include_directories(SIMx1_core PUBLIC include)

add_executable(SIMx1_sim src/main.cpp)
target_link_libraries(SIMx1_sim SIMx1_core)

enable_testing()
add_executable(testbench tests/testbench.cpp)
target_link_libraries(testbench SIMx1_core)
add_test(NAME adder_test COMMAND testbench)
