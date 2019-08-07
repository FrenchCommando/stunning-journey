//
// Created by frenchcommando on 20/07/2019.
//

#include <fstream>
#include "palindrome/PalindromicSubstring.h"
#include "gtest/gtest.h"

void test_files(const std::string& f1, const std::string& f2){
    std::string dir = "/mnt/c/Users/marti/CLionProjects/stunning-journey/tests/palindrome_test/hr_txt/";
    std::ifstream infile_test (dir + f1);
    std::ifstream infile_rep (dir + f2);
    int n;
    std::string s1, s2, s_ref;
    infile_test >> n;
    while (infile_test >> s1){
        infile_test >> s2;
        infile_rep >> s_ref;
//        std::cout << s_ref << std::endl;
//        s_ref = s_ref=="-1"?"0":std::to_string(s_ref.length());
        auto s_rep = PalindromicSubstring::solve(s1, s2);
//        std::cout << s_ref << "\t" << s_rep << std::endl;
        EXPECT_EQ(s_ref, s_rep);
    }
}

void test_num(size_t n){
    std::cout << "Test " << n << std::endl;
    test_files("test" + std::to_string(n), "rep" + std::to_string(n));
}

TEST(palindrome_substring_tests, test_hr0){
    test_num(0);
}

TEST(palindrome_substring_tests, test_hr2){
    test_num(2);
}

TEST(palindrome_substring_tests, test_hr6){
    test_num(6);
}

TEST(palindrome_substring_tests, test_hr10){
    test_num(10);
}

TEST(palindrome_substring_tests, test_hr18){
    test_num(18);
}

TEST(palindrome_substring_tests, test_hr19){
    test_num(19);
}

TEST(palindrome_substring_tests, test_hr20){
    test_num(20);
}

TEST(palindrome_substring_tests, test_hr23){
    test_num(23);
}

TEST(palindrome_substring_tests, test_hr24){
    test_num(24);
}



