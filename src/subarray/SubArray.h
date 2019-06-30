//
// Created by frenchcommando on 2019-06-30.
//

#ifndef STUNNING_JOURNEY_SUBARRAY_H
#define STUNNING_JOURNEY_SUBARRAY_H  

#include <vector>
#include <set>

class SubArray {

    std::vector<long> a;
    long m;

    // Function to return gcd of a and b
    static long gcd(long a, long b)
    {
        if (a == 0)
            return b;
        return gcd(b % a, a);
    }

// Function to find gcd of array of
// numbers
    static long gcd(std::set<long> & v)
    {
        int result = 0;
        for (const auto & i : v)
            result = gcd(i, result);
        return result;
    }
public:
    SubArray(std::vector<long> a, long m): a(std::move(a)), m(m) {}
    long solve_stupid(){
        std::vector<long> aa;
        aa.reserve(a.size());
        std::set<long> values;

        for(const auto & i : a){
            long l = i % m;
            if (l > 0) {
                aa.emplace_back(l);
                values.emplace(i);
            }
        }

        long d = gcd(values);
        long threshold = m - d;

        long current_max = 0;

        const auto update_max = [&current_max](long candidate){
            if(candidate > current_max)
                current_max = candidate;
        };

        for(size_t i = 0 ; i < aa.size(); i++){
            long s = 0;
            for (size_t j = i ; j < aa.size() ; j++) {
                s += aa[j];
                if (s >= m) // can only happen once
                    s -= m;
                update_max(s);
                if (current_max == threshold)
                    return current_max;
            }
        }

        return current_max;
    };

public:
    // Complete the maximumSum function below.
    static long maximumSum(std::vector<long>& a, long m) {
        return SubArray(a, m).solve_stupid();
    }
}; 

#endif //STUNNING_JOURNEY_SUBARRAY_H
