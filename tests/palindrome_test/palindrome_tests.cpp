//
// Created by frenchcommando on 22/06/2019.
//

#include "palindrome/Palindrome.h"
#include "gtest/gtest.h"


TEST(basic_check, test_eq){
    EXPECT_EQ(1, 1);
}

TEST(basic_check, test_neq){
    EXPECT_NE(1, 0);
}


TEST(palindrom_check, test_const){
    auto n = 10;
    std::string s (n, 'a');
    palindrome_substring_object p(s);
    for(size_t i = 0; i <= n; i++){
        EXPECT_EQ(n - i, p.longest_palindrome_substring_starting_length(i));
        EXPECT_EQ(std::string(n - i, 'a'), p.longest_palindrome_substring_starting(i));
    }
    EXPECT_EQ(0, p.longest_palindrome_substring_starting_length(n + 1));
    EXPECT_EQ("", p.longest_palindrome_substring_starting(n + 1));
}

TEST(palindrom_check, test_abcd){
    auto n = 10;
    char c = 'a';
    std::stringstream ss;
    for(size_t i = 0; i < n ; i++)
        ss << (char)(c + i);
    std::string s = ss.str();
    palindrome_substring_object p(s);

    for(size_t i = 0; i < n ; i++){
        EXPECT_EQ(1, p.longest_palindrome_substring_starting_length(i));
        EXPECT_EQ(std::string(1, (char)(c + i)), p.longest_palindrome_substring_starting(i));
    }
    EXPECT_EQ(0, p.longest_palindrome_substring_starting_length(n));
    EXPECT_EQ("", p.longest_palindrome_substring_starting(n));
}

TEST(palindrom_check, test_aba){
    std::string s = "aba";
    palindrome_substring_object p(s);

    EXPECT_EQ(3, p.longest_palindrome_substring_starting_length(0));
    EXPECT_EQ("aba", p.longest_palindrome_substring_starting(0));
    EXPECT_EQ(1, p.longest_palindrome_substring_starting_length(1));
    EXPECT_EQ("b", p.longest_palindrome_substring_starting(1));
    EXPECT_EQ(1, p.longest_palindrome_substring_starting_length(2));
    EXPECT_EQ("a", p.longest_palindrome_substring_starting(2));
    EXPECT_EQ(0, p.longest_palindrome_substring_starting_length(3));
    EXPECT_EQ("", p.longest_palindrome_substring_starting(3));
}

TEST(palindrom_check, test_abb){
    std::string s = "abb";
    palindrome_substring_object p(s);

    EXPECT_EQ(1, p.longest_palindrome_substring_starting_length(0));
    EXPECT_EQ("a", p.longest_palindrome_substring_starting(0));
    EXPECT_EQ(2, p.longest_palindrome_substring_starting_length(1));
    EXPECT_EQ("bb", p.longest_palindrome_substring_starting(1));
    EXPECT_EQ(1, p.longest_palindrome_substring_starting_length(2));
    EXPECT_EQ("b", p.longest_palindrome_substring_starting(2));
    EXPECT_EQ(0, p.longest_palindrome_substring_starting_length(3));
    EXPECT_EQ("", p.longest_palindrome_substring_starting(3));
}