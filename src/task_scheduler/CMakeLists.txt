cmake_minimum_required(VERSION 3.10)
project(task_scheduler)

set(CMAKE_CXX_STANDARD 17)

set(TASK_SCHEDULER_SOURCE_FILES
        TaskScheduler.cpp TaskScheduler.h)

add_library(task_scheduler ${TASK_SCHEDULER_SOURCE_FILES})
