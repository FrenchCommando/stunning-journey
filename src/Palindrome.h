//
// Created by frenchcommando on 22/06/2019.
//

#ifndef STUNNING_JOURNEY_PALINDROME_H
#define STUNNING_JOURNEY_PALINDROME_H


#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <algorithm>


using ItType = std::string::const_iterator;
using StringIndexType = size_t;

template <typename T>
class palindrome_substring_ending{
    using distance_type = size_t;
    using iterator_type = T;

    std::vector<std::set<iterator_type>> m; // m[i] contains the set of all the iterator_ends where each palindrome ends
    iterator_type it0;
    std::vector<size_t> longest_length;
    std::vector<iterator_type> longest_it;
public:
    explicit palindrome_substring_ending(const T& begin, const T& end)
            : it0(begin)
    {
        const auto n = std::distance(begin, end);
        m.reserve(n);
        for(auto it = begin; it != end; it++){
            auto c = *it;
            std::set<iterator_type> s;
            s.emplace(it); // current one char is a palindrome
            if (it != begin){
                if (c == *(it - 1))
                    s.emplace(it - 1); // for "AA"
                const auto& s0 = m.back();
                for(const auto& i0: s0){
                    if(i0 != begin) {
                        const auto i1 = i0 - 1;
                        if (c == *i1)
                            s.emplace(i1);
                    }
                }
            }
            m.emplace_back(s);
        }
        longest_length.reserve(n);
        longest_it.reserve(n);
        for(const auto& s : m){
            longest_it.emplace_back(*std::min_element(s.cbegin(), s.cend()));
        }

        for(auto it = begin; it != end; it++){
            longest_length.emplace_back(std::distance(longest_palindrome_substring_ending(it), it + 1));
        }
    }

    auto longest_palindrome_substring_ending(const T& it) const {
        const auto index = std::distance(it0, it);
        return longest_it.at(index);
    }

    auto longest_palindrome_substring_ending(size_t i) const{
        return std::string(longest_it.at(i), it0 + i + 1);
    }

    auto longest_palindrome_substring_ending_length(size_t i) const{
        return longest_length.at(i);
    }

    friend std::ostream &operator<<(std::ostream &os, const palindrome_substring_ending &start) {
        auto i = start.it0;
        for (const auto& v: start.m){
            for(const auto& ii: v){
                os << std::string(ii, i + 1) << ", ";
            }
            ++i;
            os << std::endl;
        }
        return os;
    }
};

class palindrome_substring_object{
    size_t n;
    palindrome_substring_ending<std::string::const_iterator> pe;
    palindrome_substring_ending<std::string::const_reverse_iterator> ps;

public:
    explicit palindrome_substring_object(const std::string& s)
            :
            n(s.length())
            , pe(s.cbegin(), s.cend())
            , ps(s.crbegin(), s.crend())
    {};

    auto length() const{
        return n;
    }

    auto longest_palindrome_substring_ending(size_t i) const
    {
        return pe.longest_palindrome_substring_ending(i);
    }
    std::string longest_palindrome_substring_starting(size_t i) const
    {
        if (i >= n)
            return "";
        return ps.longest_palindrome_substring_ending(n - i - 1);
    }

    auto longest_palindrome_substring_ending_length(size_t i) const
    {
        return pe.longest_palindrome_substring_ending_length(i);
    }
    size_t longest_palindrome_substring_starting_length(size_t i) const
    {
        if (i >= n)
            return 0;
        return ps.longest_palindrome_substring_ending_length(n - i - 1);
    }

};

class Palindrome {

};


#endif //STUNNING_JOURNEY_PALINDROME_H
