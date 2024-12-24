cmake_minimum_required(VERSION 3.8)
project(Fluid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SIZES "S(36,84)")
set(TYPES "FIXED(32,16),FAST_FIXED(32,16),DOUBLE,FLOAT")

add_executable(fluid fluid.cpp)
target_compile_definitions(fluid PUBLIC -DSIZES=${SIZES} -DTYPES=${TYPES})

add_executable(original fluid_original.cpp)