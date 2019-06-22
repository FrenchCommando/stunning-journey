cmake_minimum_required(VERSION 3.10)
project(stunning_journey)

set(CMAKE_CXX_STANDARD 17)

set(JOURNEY_SOURCE_FILES
        Palindrome.cpp Palindrome.h)

add_library(stunning_journey ${JOURNEY_SOURCE_FILES})
