//
// Created by frenchcommando on 17/07/2019.
//

#ifndef STUNNING_JOURNEY_INSULARFLOW_H
#define STUNNING_JOURNEY_INSULARFLOW_H

#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>

// Similar to Insular Graph - but uses flow techniques with Ford-Fulkerson algorithm
// get the maximum number of independent edges that can be placed on the graph
// returns whether value is at least k

// bikers are linked to the source
// bikes are linked to the target
// all capacities are equal to 1
// there exists a maximal flow that is integral
// true if max >= k

class InsularFlow {
    typedef std::tuple<char, int> NodeType;
    std::map<NodeType, std::set<NodeType>> edges;
    static constexpr NodeType source {'s', 0};
    static constexpr NodeType target {'t', 0};
    typedef std::vector<NodeType> PathType;

    explicit InsularFlow(const std::vector<std::pair<int, int>> &edges){
        std::set<NodeType> parents;
        std::set<NodeType> children;
        for(const auto& p: edges){
            NodeType parent{'p', p.first};
            NodeType child{'c', p.second};
            parents.insert(parent);
            children.insert(child);
            this->edges[parent].insert(child);
        }
        for(const auto& p : parents)
            this->edges[source].insert(p);
        for(const auto& c : children)
            this->edges[c].insert(target);
    }
    explicit InsularFlow(const std::vector<std::pair<int, int>>::iterator &edges_begin,
            const std::vector<std::pair<int, int>>::iterator &edges_end){
        std::set<NodeType> parents;
        std::set<NodeType> children;
        for(auto p = edges_begin; p!= edges_end; p++){
            NodeType parent{'p', p->first};
            NodeType child{'c', p->second};
            parents.insert(parent);
            children.insert(child);
            this->edges[parent].insert(child);
        }
        for(const auto& p : parents)
            this->edges[source].insert(p);
        for(const auto& c : children)
            this->edges[c].insert(target);
    }
    void apply_path(const PathType& p){
        auto it = p.begin();
        edges[source].erase(*it);
        // at least first parent, first child
        while(it + 1 != p.end()){
            edges[*it].erase(*(it+1));
            edges[*(it+1)].emplace(*it); // opportunity to undo
            it++;
        }
        edges[*it].erase(target);
    }
    bool find_and_apply_path(){
        // BFS search
        std::deque<PathType> queue;
        std::set<NodeType> seen;
        for(const auto & p : edges[source]){
            seen.emplace(p);
            queue.emplace_back(PathType{p});
        }
        while(not queue.empty()){
            auto p = queue[0];
            queue.pop_front();
            const auto last = *p.rbegin();
            for(const auto& n: edges[last]){
                if(n == target){
                    apply_path(p);
                    return true;
                }
                if(std::find(seen.begin(), seen.end(), n) == seen.end()){
                    auto q = p;
                    q.emplace_back(n);
                    seen.emplace(n);
                    queue.emplace_back(std::move(q));
                }
            }
        }
        return false;
    }

    bool find_and_apply_path(int k){
        std::set<int> children;
        for(const auto& p: edges){
            if(std::find(p.second.begin(), p.second.end(), target) != p.second.end()){
                const auto pp = p.first;
                if (std::get<0>(pp) == 'c')
                    children.insert(std::get<1>(pp));
            }
        }
        if(edges[source].size() < k or children.size() < k)
            return false;
        return find_and_apply_path();
    }

    int get_max_flow(){
        int count = 0;
        while(find_and_apply_path())
            count++;
        return count;
    }

    bool get_max_flow(int k){
        int count = 0;
        while(count < k and find_and_apply_path())
//        while(count < k and find_and_apply_path(k - count))
            count++;
        return count >= k;
    }

public:
    static bool solve(const std::vector<std::pair<int, int>> &edges, int k) {
        InsularFlow insularFlow(edges);
        return insularFlow.get_max_flow(k);
        return insularFlow.get_max_flow() >= k;
    };
    static bool solve(const std::vector<std::pair<int, int>>::iterator &edges_begin,
            const std::vector<std::pair<int, int>>::iterator &edges_end, int k) {
        InsularFlow insularFlow(edges_begin, edges_end);
        return insularFlow.get_max_flow(k);
        return insularFlow.get_max_flow() >= k;
    };
};

#endif //STUNNING_JOURNEY_INSULARFLOW_H
