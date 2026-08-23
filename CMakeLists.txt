cmake_minimum_required(VERSION 3.24)

project(FredMiner LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES 120)

add_executable(fredminer
    src/main.cpp
    src/cuda_device.cu
    src/stratum.cpp
)

target_include_directories(fredminer PRIVATE include)

set_target_properties(fredminer PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)

target_compile_options(fredminer PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-O3>
    $<$<COMPILE_LANGUAGE:CUDA>:-O3>
)

target_link_libraries(fredminer PRIVATE pthread)
