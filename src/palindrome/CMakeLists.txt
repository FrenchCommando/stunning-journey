cmake_minimum_required(VERSION 3.10)
project(palindrome)

set(CMAKE_CXX_STANDARD 17)

set(PALINDROME_SOURCE_FILES
        Palindrome.cpp Palindrome.h
        SuffixTree.cpp SuffixTree.h
        PalindromicSubstring.cpp PalindromicSubstring.h
        SuffixTreeInterface.h
        )

add_library(palindrome ${PALINDROME_SOURCE_FILES})
