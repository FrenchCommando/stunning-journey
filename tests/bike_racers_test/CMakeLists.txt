cmake_minimum_required(VERSION 3.10)
project(tests)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(BIKE_RACERS_TEST_FILES bike_racers_tests.cpp)
add_executable(runBikeRacersTests ${BIKE_RACERS_TEST_FILES})
target_link_libraries(runBikeRacersTests gtest gtest_main)
target_link_libraries(runBikeRacersTests bike_racers)
