//
// Created by frenchcommando on 2019-07-14.
//

#ifndef STUNNING_JOURNEY_BIKE_RACERS_H
#define STUNNING_JOURNEY_BIKE_RACERS_H

#include <numeric>
#include <algorithm>
#include "InsularGraph.h"


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

        const auto bikers_size = bikers.size();

        std::vector<std::vector<long>> m; //contains distances
        m.reserve(bikers.size());
        for(const auto& biker: bikers){
            std::vector<long> dist;
            dist.reserve(bikes.size());
            for(const auto & bike: bikes)
                dist.push_back(distance(biker, bike));
            m.push_back(std::move(dist));
        }

        const auto s_sort = [&m, bikers_size](const std::pair<int, int>& p1, const std::pair<int, int>& p2){
            return m[p2.first][p2.second - bikers_size] > m[p1.first][p1.second - bikers_size];
        };
        std::vector<std::pair<int, int>> mmm;
        mmm.reserve(bikers.size() * bikes.size());
        for(size_t i = 0; i < bikers.size(); i++)
            for(size_t j = 0; j < bikes.size(); j++)
                mmm.emplace_back(i, j + bikers_size);
        std::sort(mmm.begin(), mmm.end(), s_sort);

        const auto yield_mapping_down = [&](){
            std::set<int> bike_seen, biker_seen;
            int biker_count = 0;
            int bike_count = 0;
            auto it_down = mmm.begin();
            while(true)
            {
                biker_seen.insert(it_down->first);
                bike_seen.insert(it_down->second);
                biker_count = biker_seen.size();
                bike_count = bike_seen.size();
                if(bike_count < k or biker_count < k)
                    it_down++;
                else
                    return *it_down;
            }
        };
        auto it_down = std::lower_bound(mmm.begin(), mmm.end(), yield_mapping_down(), s_sort);
        // this is an lower-bound of the actual solution

        // Use InsularGraph object

        std::vector<std::pair<int, int>> edges (mmm.begin(), it_down);
        auto it = it_down;
        it_down = std::upper_bound(it, mmm.end(), *it, s_sort);
        std::copy(it, it_down, std::back_inserter(edges));
        while(not InsularGraph::solve(edges, k)){
            it = it_down;
            it_down = std::upper_bound(it, mmm.end(), *it, s_sort);
            std::copy(it, it_down, std::back_inserter(edges));
        }
        return m[it->first][it->second - bikers_size];
    };
}; 

#endif //STUNNING_JOURNEY_BIKE_RACERS_H
