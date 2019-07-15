cmake_minimum_required(VERSION 3.10)
project(stunning_journey)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

#add_subdirectory(libs/gflags)
#add_subdirectory(libs/glog)
add_subdirectory(libs/googletest)

add_subdirectory(src/palindrome)
add_subdirectory(tests/palindrome_test)
add_subdirectory(src/task_scheduler)
add_subdirectory(tests/task_scheduler_test)
add_subdirectory(src/subarray)
add_subdirectory(tests/subarray_test)
add_subdirectory(src/bike_racers)
add_subdirectory(tests/bike_racers_test)