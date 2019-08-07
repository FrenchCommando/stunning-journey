//
// Created by frenchcommando on 22/06/2019.
//

#include "palindrome/Palindrome.h"
#include "gtest/gtest.h"


TEST(palindrome_check, test_const){
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

TEST(palindrome_check, test_abcd){
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

TEST(palindrome_check, test_aba){
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

TEST(palindrome_check, test_abb){
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

TEST(palindrome_check, test_hr2){
    std::string s = "uxivudydgxwsgmhlracaayipsojleqhpygshcvxvchsgyphqeljospiyaacuvmeewpdwpiymwbhoxebjibxphief";
    palindrome_substring_object p(s);

    EXPECT_EQ(1, p.longest_palindrome_substring_starting_length(0));
    EXPECT_EQ("u", p.longest_palindrome_substring_starting(0));

    EXPECT_EQ(3, p.longest_palindrome_substring_starting_length(17));
    EXPECT_EQ("aca", p.longest_palindrome_substring_starting(17));
    EXPECT_EQ(41, p.longest_palindrome_substring_starting_length(18));
    EXPECT_EQ("caayipsojleqhpygshcvxvchsgyphqeljospiyaac",
              p.longest_palindrome_substring_starting(18));
    EXPECT_EQ(39, p.longest_palindrome_substring_starting_length(19));
    EXPECT_EQ("aayipsojleqhpygshcvxvchsgyphqeljospiyaa",
              p.longest_palindrome_substring_starting(19));
}

TEST(palindrome_check, test_hr6){
    std::string s = "jmbpdtwnznblwvtjrniwlbyblhppndspojrouffazpoxtqdfpjuhitvijrohavpqatofxwmksvjcvhdecxwwmosqiczjpkfamkdkjbfqflivigiwztgbybugifdyhojtjkoaqiehkjmatgyhgkjumwvalsqmcoiijvwchnenikrlizwkhktdxtqvxibctbnbxfudivicnljlnzpghgpznljlncividufxbnbtcbixvqtxdtkhkwzilrkinenhcwvjiiocmqslavwmujkghygtamjkheiqaovbvafaeqqojypxjdyxdzxbtkccesympaxxhzcaqjdoixjiivinpidpfjhvavbwpckqncjwygbbdwbqjvde";
    palindrome_substring_object p(s);

    EXPECT_EQ(1, p.longest_palindrome_substring_starting_length(0));
    EXPECT_EQ("j", p.longest_palindrome_substring_starting(0));

    EXPECT_EQ(157, p.longest_palindrome_substring_starting_length(130));
    EXPECT_EQ("oaqiehkjmatgyhgkjumwvalsqmcoiijvwchnenikrlizwkhktdxtqvxibctbnbxfudivicnljlnzpghgpznljlncividufxbnbtcbixvqtxdtkhkwzilrkinenhcwvjiiocmqslavwmujkghygtamjkheiqao", p.longest_palindrome_substring_starting(130));

    // 96
    // 130 - start palindrome
    // 157 - length palindrome
//    for(size_t i = 0; i < s.length(); i++)
//        std::cout << i << "\t" << p.longest_palindrome_substring_starting_length(i) << "\t" << p.longest_palindrome_substring_starting(i) << "\n";
//    std::cout << std::endl;
}