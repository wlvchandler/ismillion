cmake_minimum_required(VERSION 3.13)
project(ismiljson)

set(CMAKE_CXX_STANDARD 17)

add_executable(ismiljson json.cpp)

set_target_properties(ismiljson PROPERTIES
    LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS=['_parse','_malloc','_free'] -msimd128 -s ALLOW_MEMORY_GROWTH=1 -s TOTAL_MEMORY=64MB -s TOTAL_STACK=5MB -s ENVIRONMENT='web,worker'"
    COMPILE_FLAGS "-msimd128 -fno-exceptions"
)
