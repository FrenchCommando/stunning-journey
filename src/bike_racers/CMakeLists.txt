cmake_minimum_required(VERSION 3.10)
project(bike_racers)

set(CMAKE_CXX_STANDARD 17)

set(BIKE_RACERS_SOURCE_FILES
        BikeRacers.cpp BikeRacers.h InsularGraph.cpp InsularGraph.h)

add_library(bike_racers ${BIKE_RACERS_SOURCE_FILES})
