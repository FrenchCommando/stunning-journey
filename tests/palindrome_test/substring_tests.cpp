//
// Created by frenchcommando on 20/07/2019.
//


#include "gtest/gtest.h"
#include "palindrome/SuffixTreeInterface.h"


#define SUFFIX_TREE_IMPLEMENTATION 1
#if SUFFIX_TREE_IMPLEMENTATION == 0
class FakeSuffixTreeInterface : public SuffixTreeInterface{
public:
    ~FakeSuffixTreeInterface() override = default;
    char add_string(std::string s) override {return 'c';};
    [[nodiscard]] bool is_substring(const std::string &s) const override {return false;}
    [[nodiscard]] bool is_substring(const std::string &s, const char key) const override {return false;}
    [[nodiscard]] bool is_common_substring(const std::string &s) const override {return false;}
    [[nodiscard]] std::string longest_common_substring() const override {return "";}
    void print_suffix() const override{};
    void print_suffix(char key) const override {};
};
class MockSuffixTreeInterface : public FakeSuffixTreeInterface{};
#elif SUFFIX_TREE_IMPLEMENTATION == 1
#include "palindrome/SuffixTree.h"
class MockSuffixTreeInterface : public ukkonen_perso::SuffixTree{
public:
    MockSuffixTreeInterface(): ukkonen_perso::SuffixTree(){};
};
#endif


TEST(suffix_tree_check, test_suffix_aaa){
    std::string s = "aaa";
    std::unique_ptr<SuffixTreeInterface> t_ptr = std::make_unique<MockSuffixTreeInterface>();
    t_ptr->add_string(s);
//    t_ptr->print_suffix();

    EXPECT_EQ(true, t_ptr->is_substring(""));
    EXPECT_EQ(true, t_ptr->is_substring("a"));
    EXPECT_EQ(true, t_ptr->is_substring("aa"));
    EXPECT_EQ(true, t_ptr->is_substring("aaa"));
    EXPECT_EQ(false, t_ptr->is_substring("aaaa"));
    EXPECT_EQ(false, t_ptr->is_substring("b"));
    
    EXPECT_EQ(true, t_ptr->is_common_substring(""));
    EXPECT_EQ(true, t_ptr->is_common_substring("a"));
    EXPECT_EQ(true, t_ptr->is_common_substring("aa"));
    EXPECT_EQ(true, t_ptr->is_common_substring("aaa"));
    EXPECT_EQ(false, t_ptr->is_common_substring("aaaa"));
    EXPECT_EQ(false, t_ptr->is_common_substring("b"));
}

TEST(suffix_tree_check, test_suffix_abc){
    std::string s = "abc";
    std::unique_ptr<SuffixTreeInterface> t_ptr = std::make_unique<MockSuffixTreeInterface>();
    t_ptr->add_string(s);
//    t_ptr->print_suffix();

    EXPECT_EQ(true, t_ptr->is_substring(""));

    EXPECT_EQ(true, t_ptr->is_substring("a"));
    EXPECT_EQ(false, t_ptr->is_substring("aa"));
    EXPECT_EQ(true, t_ptr->is_substring("b"));
    EXPECT_EQ(false, t_ptr->is_substring("bb"));
    EXPECT_EQ(true, t_ptr->is_substring("c"));
    EXPECT_EQ(false, t_ptr->is_substring("cc"));

    EXPECT_EQ(false, t_ptr->is_substring("d"));

    EXPECT_EQ(true, t_ptr->is_substring("ab"));
    EXPECT_EQ(true, t_ptr->is_substring("bc"));
    EXPECT_EQ(false, t_ptr->is_substring("ac"));
    EXPECT_EQ(true, t_ptr->is_substring("abc"));

    EXPECT_EQ(true, t_ptr->is_common_substring(""));

    EXPECT_EQ(true, t_ptr->is_common_substring("a"));
    EXPECT_EQ(false, t_ptr->is_common_substring("aa"));
    EXPECT_EQ(true, t_ptr->is_common_substring("b"));
    EXPECT_EQ(false, t_ptr->is_common_substring("bb"));
    EXPECT_EQ(true, t_ptr->is_common_substring("c"));
    EXPECT_EQ(false, t_ptr->is_common_substring("cc"));

    EXPECT_EQ(false, t_ptr->is_common_substring("d"));

    EXPECT_EQ(true, t_ptr->is_common_substring("ab"));
    EXPECT_EQ(true, t_ptr->is_common_substring("bc"));
    EXPECT_EQ(false, t_ptr->is_common_substring("ac"));
    EXPECT_EQ(true, t_ptr->is_common_substring("abc"));
}

TEST(suffix_tree_check, test_suffix_two){
    std::string s = "abb";
    std::string ss = "acc";
    std::unique_ptr<SuffixTreeInterface> t_ptr = std::make_unique<MockSuffixTreeInterface>();
    t_ptr->add_string(s);
    t_ptr->add_string(ss);
//    t_ptr->print_suffix();
    EXPECT_EQ(true, t_ptr->is_common_substring(""));

    EXPECT_EQ(true, t_ptr->is_common_substring("a"));
    EXPECT_EQ(false, t_ptr->is_common_substring("aa"));

    EXPECT_EQ(false, t_ptr->is_common_substring("b"));
    EXPECT_EQ(false, t_ptr->is_common_substring("bb"));
    EXPECT_EQ(false, t_ptr->is_common_substring("c"));
    EXPECT_EQ(false, t_ptr->is_common_substring("cc"));
    EXPECT_EQ(false, t_ptr->is_common_substring("d"));

    EXPECT_EQ(false, t_ptr->is_common_substring("ab"));
    EXPECT_EQ(false, t_ptr->is_common_substring("bc"));
    EXPECT_EQ(false, t_ptr->is_common_substring("ac"));
    EXPECT_EQ(false, t_ptr->is_common_substring("abc"));

    EXPECT_EQ(true, t_ptr->is_substring("b"));
    EXPECT_EQ(true, t_ptr->is_substring("bb"));
    EXPECT_EQ(true, t_ptr->is_substring("c"));
    EXPECT_EQ(true, t_ptr->is_substring("cc"));
    EXPECT_EQ(false, t_ptr->is_substring("d"));

    EXPECT_EQ(true, t_ptr->is_substring("ab"));
    EXPECT_EQ(false, t_ptr->is_substring("bc"));
    EXPECT_EQ(true, t_ptr->is_substring("ac"));
    EXPECT_EQ(false, t_ptr->is_substring("abc"));
}

TEST(suffix_tree_check, test_suffix_char) {
    std::string s = "bbb";
    std::string ss = "ccc";
    std::unique_ptr<SuffixTreeInterface> t_ptr = std::make_unique<MockSuffixTreeInterface>();
    const auto b = t_ptr->add_string(s);
//    t_ptr->print_suffix();
    const auto c = t_ptr->add_string(ss);
//    t_ptr->print_suffix();

    EXPECT_EQ(true, t_ptr->is_substring("bbb", b));
    EXPECT_EQ(true, t_ptr->is_substring("ccc", c));
    EXPECT_EQ(false, t_ptr->is_substring("bbb", c));
    EXPECT_EQ(false, t_ptr->is_substring("ccc", b));
}