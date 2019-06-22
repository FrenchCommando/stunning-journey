cmake_minimum_required(VERSION 3.10)
project(tests)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(JOURNEY_TEST_FILES journey_tests.cpp)
add_executable(runJourneyTests ${JOURNEY_TEST_FILES})

target_link_libraries(runJourneyTests gtest gtest_main)
target_link_libraries(runJourneyTests stunning_journey)
