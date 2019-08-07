//
// Created by frenchcommando on 20/07/2019.
//

#ifndef STUNNING_JOURNEY_PALINDROMIC_SUBSTRING_H
#define STUNNING_JOURNEY_PALINDROMIC_SUBSTRING_H


#include <map>
#include "palindrome/SuffixTree.h"
#include "palindrome/Palindrome.h"


constexpr auto compare_string = [](const std::string & s1, const std::string & s2){
    if(s1.length() > s2.length())
        return s1;
    if(s1.length() < s2.length())
        return s2;
    if(s1 < s2)
        return s1;
    return s2;
};

class PalindromicSubstring {
    static std::string solve_palindromic_substring(const std::string &s1, const std::string &s2) {
        std::vector<char> common_data;
        std::set<char> ss1, ss2;
        for(const auto& c: s1)
            ss1.insert(c);
        for(const auto& c: s2)
            ss2.insert(c);
        set_intersection(ss1.begin(), ss1.end(), ss2.begin(), ss2.end(),
                std::back_inserter(common_data));
        if(common_data.empty())
            return "";

        ukkonen_perso::SuffixTree t;
        const auto c1 = t.add_string(s1);
        const auto c2 = t.add_string(s2);
//        t.print_suffix();
        std::map<char, palindrome_substring_object> m{
                {c1, palindrome_substring_object(s1)},
                {c2, palindrome_substring_object(s2)},
        };
        const auto p_string_length = [&](char c, int i){
            return m.at(c).longest_palindrome_substring_starting_length(i);
        };
        const auto p_string_value = [&](char c, int i){
            return m.at(c).longest_palindrome_substring_starting(i);
        };
        return t.longest_common_substring_plus_palindrome(p_string_length, p_string_value, compare_string);
    };

public:
    static std::string solve(const std::string &s1, const std::string &s2) {
        std::string rs2(s2.crbegin(), s2.crend());
        const auto ss = solve_palindromic_substring(s1, rs2);
        if(ss.empty())
            return "-1";
        return ss;
    };
};


#endif //STUNNING_JOURNEY_PALINDROMIC_SUBSTRING_H
