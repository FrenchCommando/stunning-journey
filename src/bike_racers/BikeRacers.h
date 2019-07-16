//
// Created by frenchcommando on 2019-07-14.
//

#ifndef STUNNING_JOURNEY_BIKE_RACERS_H
#define STUNNING_JOURNEY_BIKE_RACERS_H

#include <numeric>
#include <algorithm>
#include <iostream>
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

        std::vector<std::vector<long>> m; //contains distances
        std::vector<std::vector<int>> priority;
        m.reserve(bikers.size());
        priority.reserve(bikers.size());
        for(const auto& biker: bikers){
            std::vector<long> dist;
            std::vector<int> prio(bikes.size());
            dist.reserve(bikes.size());
            for(const auto & bike: bikes)
                dist.push_back(distance(biker, bike));
            std::iota(prio.begin(), prio.end(), 0);
            std::sort(prio.begin(), prio.end(), [dist](int a, int b) {
                return dist[a] < dist[b];
            });
            m.push_back(std::move(dist));
            priority.push_back(std::move(prio));
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

        const auto yield_mapping_up = [&](){
            std::vector<bool> taken(bikes.size(), false);
            std::map<int, int> mapping;
            for(size_t i = 0 ; i < bikers.size(); i++){ // assign all bikers
                size_t index = 0;
                while(taken[priority[i][index]])
                    index++;
                const auto chosen = priority[i][index];
                taken[chosen] = true;
                mapping[i] = chosen;
            }
            std::vector<std::pair<int, int>> mmm_one;
            mmm_one.reserve(bikers.size());
            for(const auto & u: mapping)
                mmm_one.emplace_back(std::pair<int, int>{u.first, u.second});
            std::sort(mmm_one.begin(), mmm_one.end(), s_sort);
            return mmm_one[k - 1];
        };
        auto it_up = std::lower_bound(mmm.begin(), mmm.end(), yield_mapping_up(), s_sort);
        // this is an upper-bound of the actual solution

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

        if (m[it_down->first][it_down->second] == m[it_up->first][it_up->second])
            return m[it_down->first][it_down->second];

        // Use InsularGraph object

        std::vector<std::pair<int, int>> edges (mmm.begin(), it_down);
        auto it = it_down;
        it_down = std::upper_bound(it, mmm.end(), *it, s_sort);
        std::copy(it, it_down, std::back_inserter(edges));
        while(not InsularGraph::solve(edges, k)){
            it = it_down;
            it_down = std::upper_bound(it, mmm.end(), *it, s_sort);
            std::copy(it, it_down, std::back_inserter(edges));
            std::cout << m[it->first][it->second] << std::endl;
        }
        return m[it->first][it->second];
    };
}; 

#endif //STUNNING_JOURNEY_BIKE_RACERS_H
