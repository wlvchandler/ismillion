# CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
project(ismiljson)

set(CMAKE_CXX_STANDARD 17)

add_executable(ismiljson json.cpp)

set_target_properties(ismiljson PROPERTIES 
    LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS=['_parse'] -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -msimd128"
    COMPILE_FLAGS "-msimd128"
)
