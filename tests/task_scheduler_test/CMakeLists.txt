cmake_minimum_required(VERSION 3.10)
project(tests)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TASK_SCHEDULER_TEST_FILES task_scheduler_tests.cpp)
add_executable(runTaskSchedulerTests ${TASK_SCHEDULER_TEST_FILES})

target_link_libraries(runTaskSchedulerTests  gtest gtest_main)
target_link_libraries(runTaskSchedulerTests task_scheduler)
