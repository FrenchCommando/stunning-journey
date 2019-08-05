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
#include "SuffixTreeInterface.h"

namespace ukkonen_perso{

    struct Node;
    using NodePtr = std::unique_ptr<Node>;
    using NodePtrRaw = Node*;
    using StringKey = char;
    using IndexType = int;
    constexpr int MAX_CHAR = 256;

    constexpr StringKey escaped_char [] = "!@#$%^&*()_+{}:<>?-=[];',./";
    // zero char at the end
    constexpr StringKey root_char {' '}; // space not zero
    constexpr IndexType root_start {-1};

    struct Node {
        std::array<NodePtr, MAX_CHAR> children {nullptr};
        StringKey key;

        IndexType start;
        IndexType *end;  // leaf node - points to fixed_end is fixed
        IndexType fixed_end {-1};  // internal node

        // this is just used during the construction
        NodePtrRaw suffixLink {nullptr};

        // for common substring keys
        std::array<bool, MAX_CHAR> has_key{false};
        bool has_all_keys {false};
        // invalidate value when adding new string tot he tree


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

        void fill_node_keys(const std::string& keys, std::set<char>& found){
            if(is_leaf()) { // is leaf
                found.insert(key);
            }
            else{
                auto r_found = found;
                std::swap(found, r_found);
                for (int i = 0; i < MAX_CHAR; i++)
                {
                    if(children.at(i) != nullptr){
                        auto c_found = found;
                        children.at(i)->fill_node_keys(keys, c_found);
                        for(const auto & u: c_found)
                            r_found.insert(u);
                    }
                }
                std::swap(found, r_found);
            }
//            has_key.at(key) = true;
            for(const auto & b: found)
                has_key.at(b) = true;
            has_all_keys = std::all_of(keys.cbegin(), keys.cend(),
                    [h=&has_key](char s){ return h->at(s); });
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

    };

    class SuffixTree : public SuffixTreeInterface{

        NodePtr root;
        std::array<const char*, MAX_CHAR> text{nullptr};
        std::array<IndexType, MAX_CHAR> text_end{-1};
        std::string keys{""};
        std::array<std::string, MAX_CHAR> text_str{""};

        void fill_node_keys(){
            std::set<char>s {};
            root->fill_node_keys(keys, s);
        };

        int edgeLength(const NodePtrRaw& n) {
            if(n == root.get())
                return 0;
            return *(n->end) - (n->start) + 1;
        }

        void extend_suffix_tree(const char k, const char* t, IndexType& end, const size_t size){
            // c is the key for the string to append
            int remainder = 0;
            NodePtrRaw active_node = root.get();
            IndexType active_len = 0;
            IndexType active_e = 0;
            // follow active edge along suffix link - then reduce length

            const auto walk_down = [&](){
                const auto active_edge_length = edgeLength(active_node);
                while (active_node != root.get()
                       and active_len >= active_edge_length)
                {
                    active_e += active_edge_length;
                    active_len -= active_edge_length;
                    active_node = active_node->children.at(text.at(k)[active_e]).get();
                }
            };


            for(int i = 0 ; i < size; i++){
                end = i;
                const auto c = t[end];
                NodePtrRaw lastNewNode = nullptr;

                const auto build_node = [&lastNewNode, &remainder, k](IndexType start, IndexType* end){
                    auto p = std::make_unique<Node>(k, start, end);
                    if (lastNewNode != nullptr)
                        lastNewNode->suffixLink = p.get();
                    lastNewNode = p.get();
                    remainder--;
                    return std::move(p);
                };

                remainder++;
                while(remainder > 0) {
                    if (active_len == 0)
                        active_e = end;

                    const auto active_edge = text.at(k)[active_e];
                    if (active_node->children.at(active_edge).get() == nullptr)
                        active_node->children.at(active_edge) = build_node(end, &end);
                    else {
                        walk_down(); //observation 2
                        auto nxt = active_node->children.at(text.at(k)[active_e]).get();
                        if (text.at(nxt->key)[nxt->start + active_len] == c) { //observation 1
                            active_len++;
                            if (lastNewNode != nullptr)
                                lastNewNode->suffixLink = active_node;
                            break;
                        }
                        IndexType split_end = nxt->start + active_len;
                        auto split_c = text.at(nxt->key)[split_end];
                        auto ref_c = text.at(k)[end];
                        auto active_children = std::move(nxt->children);

                        //New internal node
                        auto old_end = nxt->end;
                        IndexType old_fixed_end = nxt->fixed_end;
                        nxt->fix_end(split_end - 1);
                        nxt->children[split_c] = std::make_unique<Node>(
                                nxt->key, split_end, old_end);
                        if(old_fixed_end != -1)
                            nxt->children[split_c]->fix_end(old_fixed_end);
                        nxt->children[split_c]->children = std::move(active_children);

                        //New leaf coming out of new internal node
                        nxt->children[ref_c] = build_node(end, &end);
                    }

                    if (active_node == root.get() && active_len > 0) { //rule 1
                        active_len--;
                        active_e = end - remainder + 1;
                    } else{
                        if(active_node->suffixLink != nullptr)
                            active_node = active_node->suffixLink;
                        else
                            active_node = root.get();
                    }
                }
            }
        }

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
            if (n.children.at(c) == nullptr)
                return false;
            return is_substring_with_pred(it, end, *n.children.at(c), f);
        }

        [[nodiscard]] std::string get_string(const Node& n) const{
            if(n.key == root_char)
                return "";
            std::string s;
            for(IndexType i = n.start; i <= *n.end; i++)
                s += text.at(n.key)[i];
            return s;
        }

        void print(const Node& n, std::string s, const std::function<bool(const Node&)> & f) const{
            const auto ss = std::move(s) + get_string(n);
            if(n.is_leaf() and f(n)){
                printf("%s", ss.c_str());
                std::cout << '\n';
            }
            else{
                for (int i = 0; i < MAX_CHAR; i++)
                {
                    if(n.children.at(i) != nullptr){
                        print(*n.children.at(i), ss, f);
                    }
                }
            }
        }

    public:
        explicit SuffixTree()
                : root(std::move(Node::build_root()))
        {};

        ~SuffixTree() override = default;
        char add_string(std::string s) override {
//            std::cout << std::endl;
//            std::cout << s << "\t" << keys.size() << "\t" << (sizeof escaped_char) << std::endl;
            if(keys.size() > (sizeof escaped_char)) {
                throw std::range_error("Can't build string - max number of escaped chars reached");
            }
            const auto k = escaped_char[keys.size()];
            keys += k;
//            std::cout << "Keys\t" << keys << std::endl;

            text_str.at(k) = std::move(s);
            text_str.at(k) += k;
            text.at(k) = text_str.at(k).c_str();
            extend_suffix_tree(k, text.at(k), text_end.at(k), text_str.at(k).length());
            fill_node_keys();
            return k;
        }
        [[nodiscard]] bool is_substring(const std::string &s) const override {
            const auto f = [](const Node& n) -> bool {return true;};
            return is_substring_with_pred(s.cbegin(), s.cend(), *root, f);
        }
        [[nodiscard]] bool is_substring(const std::string &s, const char key) const override {
            const auto f = [key](const Node& n) -> bool {return n.has_key.at(key);};
            return is_substring_with_pred(s.cbegin(), s.cend(), *root, f);        }
        [[nodiscard]] bool is_common_substring(const std::string &s) const override {
            const auto f = [](const Node& n) -> bool {return n.has_all_keys;};
            return is_substring_with_pred(s.cbegin(), s.cend(), *root, f);        }
        [[nodiscard]] std::string longest_common_substring() const override {
            return "Me = Me - Me Me";
        }

        void print_suffix() const override {
            const auto f = [](const Node& n) -> bool {return true;};
            print(*root, "", f);
            std::cout << std::endl;
        };
        void print_suffix(char key) const override {
            const auto f = [key](const Node& n) -> bool {return n.has_key.at(key);};
            print(*root, "", f);
            std::cout << std::endl;
        };

    };

}

#endif //STUNNING_JOURNEY_SUFFIX_TREE_H
