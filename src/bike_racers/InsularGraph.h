//
// Created by frenchcommando on 15/07/2019.
//

#ifndef STUNNING_JOURNEY_INSULARGRAPH_H
#define STUNNING_JOURNEY_INSULARGRAPH_H


#include <vector>
#include <map>
#include <set>

// get the maximum number of independent edges that can be placed on the graph
// returns whether value is at least k
class InsularGraph {
    static std::map<int, std::vector<int>> build_count_map(const std::vector<std::pair<int, int>> &edges){
        std::map<int, std::vector<int>> m;
        for(const auto & p: edges){
            m[p.first].push_back(p.second);
            m[p.second].push_back(p.first);
        }
        return m;
    }
    static bool exclude_single_node(std::map<int, std::vector<int>>& count_map,
        std::set<int>& excluded){
        for(const auto& p : count_map){
            if(p.second.size() == 1){
                excluded.insert(p.first);
                excluded.insert(p.second[0]);
                count_map.erase(p.second[0]);
                count_map.erase(p.first);
                return true;
            }
        }
        return false;
    }

    static std::vector<std::pair<int, int>> exclude_edges(
            const std::vector<std::pair<int, int>>& edges,
            const std::set<int>& excluded){
        std::vector<std::pair<int, int>> remaining_edges;
        for(const auto & p: edges) {
            if (excluded.find(p.first) == excluded.end()
                and excluded.find(p.second) == excluded.end())
                remaining_edges.push_back(p);
        }
        return remaining_edges;
    }

public:
    static bool solve(const std::vector<std::pair<int, int>> &edges, int k){
        if(k <= 0)
            return true;
        if(edges.empty())
            return false;
        auto count_map = build_count_map(edges);
        std::set<int> excluded;
        if(exclude_single_node(count_map, excluded)){
//        if (not excluded.empty()){
            const auto remaining_edges = exclude_edges(edges, excluded);
            return solve(remaining_edges, k - 1);
        }
        // the structure is complicated
        const auto p = count_map.begin();
        const auto i = p->first;
        for(const auto& j : p->second){
            const auto remaining_edges = exclude_edges(edges, {i, j});
            if(solve(remaining_edges, k - 1))
                return true;
        }
        return false;
    }
};


#endif //STUNNING_JOURNEY_INSULARYGRAPH_H
