//
// Created by frenchcommando on 2019-07-14.
//

#ifndef STUNNING_JOURNEY_BIKE_RACERS_H
#define STUNNING_JOURNEY_BIKE_RACERS_H

#include <numeric>
#include <algorithm>
//#include <iostream>
#include "InsularGraph.h"
#include "InsularFlow.h"


class BikeRacers {
    template<typename T>
    static long distance(T& biker, T& bike){
        long s = 0;
        for(size_t d = 0; d < biker.size(); d++) {
            long ss = biker[d] - bike[d];
            s += ss * ss;
        }
        return s;
    }

public:
    static long solve(std::vector<std::vector<int>>& bikers, std::vector<std::vector<int>>& bikes, int k){
        if(bikers.size() > bikes.size())
            return solve(bikes, bikers, k);
        // assume now there are more bikes than bikers
        if(k == 0)
            return 0;

        std::vector<std::vector<long>> m; //contains distances
        m.reserve(bikers.size());
        for(const auto& biker: bikers){
            std::vector<long> dist;
            dist.reserve(bikes.size());
            for(const auto & bike: bikes)
                dist.push_back(distance(biker, bike));
            m.push_back(std::move(dist));
        }

        const auto s_sort = [&m](const std::pair<int, int>& p1, const std::pair<int, int>& p2){
            return m[p2.first][p2.second] > m[p1.first][p1.second];
        };
        std::vector<std::pair<int, int>> mmm;
        mmm.reserve(bikers.size() * bikes.size());
        for(size_t i = 0; i < bikers.size(); i++)
            for(size_t j = 0; j < bikes.size(); j++)
                mmm.emplace_back(i, j);
        std::sort(mmm.begin(), mmm.end(), s_sort);

        // Use InsularGraph object
        const auto b = mmm.begin();
        const auto value = [k, b, &m](const std::vector<std::pair<int, int>>::iterator& it) -> bool {
//            std::cout << std::distance(b, it) << "\t" << m[it->first][it->second] << std::endl;
//            std::vector<std::pair<int, int>> v (b, it);
            return InsularFlow::solve(b, it, k);
        };
        auto it = mmm.begin(); // binary_search comp function doesn't take iterator as argument
        auto top = mmm.end();
        while (top != it){
            if(top == it+1){
                break;
            }
            auto mid = it + std::distance(it, top) / 2;
//            std::cout << "Mid" << "\t" << std::distance(mmm.begin(), mid) << std::endl;
            if(value(mid)){
                top = mid;
            }
            else{
                it = mid;
            }
        }
        return m[it->first][it->second];
    };
}; 

#endif //STUNNING_JOURNEY_BIKE_RACERS_H
