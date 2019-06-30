cmake_minimum_required(VERSION 3.10)
project(subarray)

set(CMAKE_CXX_STANDARD 17)

set(SUBARRAY_SOURCE_FILES
        SubArray.cpp SubArray.h)

add_library(subarray ${SUBARRAY_SOURCE_FILES})
