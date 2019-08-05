//
// Created by frenchcommando on 03/08/2019.
//

#ifndef STUNNING_JOURNEY_SUFFIX_TREE_INTERFACE_H
#define STUNNING_JOURNEY_SUFFIX_TREE_INTERFACE_H

#include <string>

class SuffixTreeInterface{
public:
    virtual ~SuffixTreeInterface() = default;
    virtual char add_string(std::string s) = 0;
    [[nodiscard]] virtual bool is_substring(const std::string& s) const = 0;
    [[nodiscard]] virtual bool is_substring(const std::string& s, char key) const = 0;
    [[nodiscard]] virtual bool is_common_substring(const std::string& s) const = 0;
    [[nodiscard]] virtual std::string longest_common_substring() const = 0;
//    [[nodiscard]] virtual auto highest_value_substring() const = 0;
    virtual void print_suffix() const = 0;
    virtual void print_suffix(char key) const = 0;
};

#endif //STUNNING_JOURNEY_SUFFIX_TREE_INTERFACE_H
