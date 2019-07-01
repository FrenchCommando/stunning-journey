//
// Created by frenchcommando on 2019-06-30.
//

#ifndef STUNNING_JOURNEY_SUBARRAY_H
#define STUNNING_JOURNEY_SUBARRAY_H  

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

class SubArray {

    std::vector<long> a;
    long m;

    long current_max = 0;

    void preprocess(){
        auto p = a.begin();
        for(const auto & i : a){
            long l = i;
            if(l >= m)
                l %= m;
            if (l != 0) {
                *p++ = l;
            }
        }
        a.resize(std::distance(a.begin(), p));

    }

    explicit SubArray(std::vector<long> a, long m): a(std::move(a)), m(m){
        preprocess();
    };

    inline bool update_max(long candidate){
        if(candidate == m - 1)
            return true;
        if(candidate > current_max)
            current_max = candidate;
        return false;
    };

    long solve_stupid(){
        for(auto it = a.begin(); it != a.end(); it++){
            long s = 0;
            for (auto it0 = it; it0 != a.end(); it0++){
                s += *it0;
                if (s >= m) // can only happen once
                    s -= m;
                if (s == m - 1)
                    return s;
                update_max(s);
            }
        }
        return current_max;
    };

    long solve_prefix(){
        std::set<long> prefix;
        long p = 0;
        for(const auto & i : a){
            p += i; // increment prefix
            if(p >= m)
                p -= m;
            if (update_max(p))
                return m - 1;
            auto u = prefix.insert(p);
            auto it = ++(u.first);
            if(it != prefix.end()){
                long c = p - *it + m;
                if(c >= m)
                    c -= m;
                if (update_max(c))
                    return m - 1;
            }
        }
        return current_max;
    };


public:
    static long maximumSum(std::vector<long>& a, long m) {
        SubArray s(a, m);
//        std::cout << m << std::endl;
        return s.solve_prefix();
        return s.solve_stupid();
    }
}; 

#endif //STUNNING_JOURNEY_SUBARRAY_H
