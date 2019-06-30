//
// Created by frenchcommando on 2019-06-30.
//

#ifndef STUNNING_JOURNEY_SUBARRAY_H
#define STUNNING_JOURNEY_SUBARRAY_H  

#include <vector>

class SubArray {

    std::vector<long> a;
    long m;
public:
    SubArray(std::vector<long> a, long m): a(std::move(a)), m(m) {}
    long solve_stupid() const{
        long current_max = 0;

        const auto update_max = [&current_max](long candidate){
            if(candidate > current_max)
                current_max = candidate;
        };

#define update_max_macro(candidate) update_max(candidate);\
        if (current_max == m - 1)\
            return current_max;

        for(size_t i = 0 ; i < a.size(); i++){
            for (size_t j = i ; j < a.size() ; j++){
                long s = 0;
                for(size_t k = i; k <= j ; k++){
                    s += a[k] % m;
                }
                s = s % m;
                update_max_macro(s)
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
        