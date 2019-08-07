//
// Created by frenchcommando on 19/07/2019.
//

#ifndef STUNNING_JOURNEY_SUFFIX_TREE_H
#define STUNNING_JOURNEY_SUFFIX_TREE_H


// Let's do Ukkonen - at least try
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <set>
#include <map>
#include <sstream>
#include "SuffixTreeInterface.h"

namespace ukkonen_perso{

    struct Node;  // forward declaration
    using NodePtr = std::unique_ptr<Node>;
    using NodePtrRaw = Node*;
    using StringKey = char;
    using IndexType = int;
    constexpr int MAX_CHAR = 256;

    constexpr StringKey escaped_char [] = "!@#$%^&*()_{}:<>?-=[];',./";
    // zero char at the end
    constexpr StringKey root_char {'+'}; // not zero - anything else is easier to read
    constexpr IndexType root_start {-1};

    struct Node {
        std::map<char, NodePtr> children;
        StringKey key;

        IndexType start;
        IndexType *end;  // leaf node - points to fixed_end is fixed
        IndexType fixed_end {-1};  // internal node

        // this is just used during the construction
        NodePtrRaw suffixLink {nullptr};

        // for common substring keys
        std::set<char> has_key;
        bool has_all_keys {false};

        // to route longest common substring - or other criteria
        char c_longest {root_char}; // which child to choose
        int i_longest {0};
        std::map<StringKey, std::vector<IndexType>> string_index {}; // index for end of node


        explicit Node(StringKey key, IndexType start, IndexType* end)
                :
                key(key)
                , start(start)
                , end(end)
        {}

        void fix_end(IndexType fixed_end_in){
            fixed_end = fixed_end_in;
            end = &fixed_end;
        }

        void fill_node_keys(const std::set<char>& keys, std::set<char>& found){
            if(is_leaf()) { // is leaf
                found.insert(key);
            }
            else{
                auto r_found = found;
                std::swap(found, r_found);
                for(const auto & p: children){
                    auto c_found = found;
                    p.second->fill_node_keys(keys, c_found);
                    for(const auto & u: c_found)
                        r_found.insert(u);
                }
                std::swap(found, r_found);
            }
//            has_key.at(key) = true;
            for(const auto & b: found)
                has_key.insert(b);
            has_all_keys = (has_key == keys);
        }

        int fill_longest_branch(){
            if(is_leaf()){
                i_longest = string_length();
                return i_longest;
            }

            for(const auto & p: children){
                if(p.second->has_all_keys){
                    const auto i_candidate = p.second->fill_longest_branch();
                    if(i_candidate > i_longest){
                        i_longest = i_candidate;
                        c_longest = p.first;
                    }
                }
            }
            i_longest += string_length();
            return i_longest;
        }

        int longest_common_substring_plus_palindrome_length(
                const std::function<int(char, int)>& p_string){
            if(is_leaf()){ // no common substring
                i_longest = 0;
                string_index[key] = std::vector<IndexType>{*end};
                return i_longest;
            }
            for(const auto & p: children){
                const auto i_candidate =
                        p.second->longest_common_substring_plus_palindrome_length(p_string)
                        + string_length() * 2;
                if(p.second->has_all_keys){
                    if(i_candidate > i_longest){
                        i_longest = i_candidate;
                        c_longest = p.first;
                    }
                }
                for(const auto & pp : p.second->string_index)
                    for(const auto s: pp.second)
                        string_index[pp.first].emplace_back(s - p.second->string_length());
            }
            if(not is_root() and has_all_keys){
                for(const auto & pp : string_index){
                    for(const auto s: pp.second){
                        const auto i_candidate = p_string(pp.first, s + 1) + string_length() * 2;
                        if(i_candidate > i_longest){
                            i_longest = i_candidate;
                            c_longest = root_char;
                        }
                    }
                }
            }
            return i_longest;
        }

        static NodePtr build_root(){
            return std::make_unique<Node>(root_char, root_start, nullptr);
        }

        [[nodiscard]] bool is_leaf() const{
            return end != &fixed_end and key != root_char;
        }

        [[nodiscard]] bool is_root() const{
            return key == root_char;
        }

        [[nodiscard]] int string_length() const{
            if(!is_root())
                return *end - start + 1;
            return 0;
        }

    };

    class SuffixTree : public SuffixTreeInterface{

        NodePtr root;
        std::array<const char*, MAX_CHAR> text{nullptr};
        std::array<IndexType, MAX_CHAR> text_end{-1};
        std::set<char> keys;
        std::array<std::string, MAX_CHAR> text_str{""};

        [[nodiscard]] std::string get_string(const Node& n) const{
            if(n.key == root_char)
                return "";
//                return std::string(1, static_cast<char>(root_char));
            std::string s;
            for(IndexType i = n.start; i <= *n.end; i++)
                s += text.at(n.key)[i];
            return s;
        }

        void extend_suffix_tree(const char k, const char* t, IndexType& end, const size_t size){
            // c is the key for the string to append
            int remainder = 0;
            NodePtrRaw active_node = root.get();
            root->suffixLink = root.get();
            IndexType active_len = 0;
            IndexType active_e = 0;

            for(int i = 0 ; i < size; i++){
                end = i;
                const auto c = t[end];
                NodePtrRaw lastNewNode = nullptr;

                remainder++;
                while(remainder > 0) {
                    if (active_len == 0)
                        active_e = end;

                    const auto active_edge = text.at(k)[active_e];
                    if (active_node->children.find(active_edge) == active_node->children.end()){
                        active_node->children[active_edge] = std::make_unique<Node>(k, end, &end);
                        active_node->children[active_edge]->suffixLink = root.get();
                        if (lastNewNode != nullptr)
                            lastNewNode->suffixLink = active_node;
                        lastNewNode = active_node;
                        remainder--;
                    }
                    else {
                        if(auto active_edge_length = active_node->children[active_edge]->string_length();
                        active_len >= active_edge_length) {
                            active_e += active_edge_length;
                            active_len -= active_edge_length;
                            active_node = active_node->children[active_edge].get();
                            continue;
                        }
                        if (text.at(active_node->children[active_edge]->key)
                        [active_node->children[active_edge]->start
                        + active_len] == c) {
                            active_len++;
                            break;
                        }
                        auto nxt = std::move(active_node->children[active_edge]);

                        active_node->children[active_edge] = std::make_unique<Node>(nxt->key, nxt->start, nullptr);
                        const auto split_node = active_node->children[active_edge].get();
                        if (lastNewNode != nullptr) {
                            lastNewNode->suffixLink = split_node;
                        }
                        lastNewNode = split_node;

                        split_node->fix_end(nxt->start + active_len - 1);
                        remainder--;

                        const auto ref_c = text.at(k)[end];
                        split_node->children[ref_c] = std::make_unique<Node>(k, end, &end); // this is a leaf
                        split_node->children[ref_c]->suffixLink = root.get();

                        const auto split_c = text.at(nxt->key)[nxt->start + active_len];
                        nxt->start += active_len;
                        split_node->children[split_c] = std::move(nxt);
                    }

                    if (active_node == root.get() && active_len > 0) {
                        active_len--;
                        active_e = end - remainder + 1;
                    }
                    else{
                        active_node = active_node->suffixLink;
                    }
                }
                if (lastNewNode != nullptr)
                    lastNewNode->suffixLink = active_node;
            }
        }

        void fill_node_keys() const {
            std::set<char>s {};
            root->fill_node_keys(keys, s);
        };

        bool is_substring_with_pred(const std::string::const_iterator& begin,
                                    const std::string::const_iterator& end,
                                    const Node& n,
                                    const std::function<bool(const Node&)> & f)
                                    const {
            if(not f(n))
                return false;
            auto it = begin;
            if(n.key != root_char){
                IndexType i = n.start;
                while(i != *n.end + 1 and it!= end){
                    if(text.at(n.key)[i] != *it)
                        return false;
                    i++;
                    it++;
                }
            }
            if(it == end)
                return true;
            const auto c = *it;
            if (n.children.find(c) == n.children.end())
                return false;
            return is_substring_with_pred(it, end, *(n.children.at(c)), f);
        }

        void fill_longest_branch() const {
            root->fill_longest_branch();
        }

        void print(const Node& n, std::string s, const std::function<bool(const Node&)> & f) const{
            const auto ss = std::move(s) + get_string(n);
            if(n.is_leaf() and f(n)){
                printf("%s", ss.c_str());
                std::cout << '\n';
            }
            else{
                for(const auto & p: n.children)
                    print(*p.second, ss, f);
            }
        }

        void print_longest(const Node& n) const{
            std::cout << "NextLongestString\t" << n.c_longest << "\t" << n.i_longest << "\t" << get_string(n);
            std::cout << '\n';
            for(const auto & p: n.children)
                std::cout << p.first << " ";
            std::cout << '\n';
            std::cout << "KeyStartEnd\t" << n.key << "\t" << n.start << "\t";
            if(n.end != nullptr)
                std::cout << *n.end;
            std::cout << '\n';
            std::cout << '\n';
            if(n.children.find(n.c_longest) != n.children.end())
                print_longest(*n.children.at(n.c_longest).get());
        }

    public:
        explicit SuffixTree()
                : root(std::move(Node::build_root()))
        {};

        ~SuffixTree() override = default;
        char add_string(std::string s) override {
            if(keys.size() > (sizeof escaped_char)) {
                throw std::range_error("Can't build string - max number of escaped chars reached");
            }
            const auto k = escaped_char[keys.size()];
            keys.insert(k);

            text_str.at(k) = std::move(s);
            text_str.at(k) += k;
            text.at(k) = text_str.at(k).c_str();

            extend_suffix_tree(k, text.at(k), text_end.at(k), text_str.at(k).length());
            return k;
        }
        [[nodiscard]] bool is_substring(const std::string &s) const override {
            const auto f = [](const Node& n) -> bool {return true;};
            return is_substring_with_pred(s.cbegin(), s.cend(), *root, f);
        }
        [[nodiscard]] bool is_substring(const std::string &s, const char key) const override {
            const auto f = [key](const Node& n) -> bool {return n.has_key.find(key) != n.has_key.end();};
            fill_node_keys();
            return is_substring_with_pred(s.cbegin(), s.cend(), *root, f);
        }
        [[nodiscard]] bool is_common_substring(const std::string &s) const override {
            const auto f = [](const Node& n) -> bool {return n.has_all_keys;};
            fill_node_keys();
            return is_substring_with_pred(s.cbegin(), s.cend(), *root, f);
        }
        [[nodiscard]] std::string longest_common_substring() const override {
            fill_node_keys();
            fill_longest_branch();
            NodePtrRaw node = root.get();
            std::string s;
            while(node->children.find(node->c_longest) != node->children.end()){
                s += get_string(*node);
                node = node->children[node->c_longest].get();
            }
            s += get_string(*node);
            return s;
        }

        [[nodiscard]] std::string longest_common_substring_plus_palindrome(
                const std::function<int(char, int)>& p_string_length,
                const std::function<std::string(char, int)>& p_string_value,
                const std::function<std::string(const std::string&, const std::string&)>& string_compare
                ) const{
            fill_node_keys();
            const auto rep_int = root->longest_common_substring_plus_palindrome_length(p_string_length);
            std::cout << rep_int << std::endl;
            std::stringstream ss;
            auto n = root.get();
            while(n->c_longest != root_char){
                n = n->children[n->c_longest].get();
                ss << get_string(*n);
            }
            const auto s1 = ss.str();
            const auto m = n->string_index;
            std::string s_pal_candidate;
            for(const auto & p : m)
                for(const auto i: p.second)
                    s_pal_candidate = string_compare(p_string_value(p.first, i + 1), s_pal_candidate);
            return s1 + s_pal_candidate + std::string(s1.crbegin(), s1.crend());
        }

        void print_suffix() const override {
            const auto f = [](const Node& n) -> bool {return true;};
            print(*root, "", f);
            std::cout << std::endl;
        };
        void print_suffix(char key) const override {
            const auto f = [key](const Node& n) -> bool {return n.has_key.find(key) != n.has_key.end();};
            print(*root, "", f);
            std::cout << std::endl;
        };

    };

}

#endif //STUNNING_JOURNEY_SUFFIX_TREE_H
