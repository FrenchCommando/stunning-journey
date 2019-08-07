cmake_minimum_required(VERSION 3.10)
project(tests)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(PALINDROME_TEST_FILES
        palindrome_tests.cpp
        palindrome_substring_tests.cpp
        substring_tests.cpp
        )
add_executable(runPalindromeTests ${PALINDROME_TEST_FILES})

target_link_libraries(runPalindromeTests gtest gtest_main)
target_link_libraries(runPalindromeTests palindrome)
