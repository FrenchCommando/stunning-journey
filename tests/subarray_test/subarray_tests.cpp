//
// Created by frenchcommando on 2019-06-30.
//
#include "subarray/SubArray.h"
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <fstream>


void test_files(const std::string& f1, const std::string& f2){
    std::string dir = "/mnt/c/Users/marti/CLionProjects/stunning-journey/tests/subarray_test/hr_txt/";
    std::ifstream infile_test (dir + f1);
    std::ifstream infile_rep (dir + f2);
    int q;
    int s_ref;
    infile_test >> q;

    for (int q_itr = 0; q_itr < q; q_itr++) {
        int n;
        long m;
        infile_test >> n >> m;
        std::vector<long> a(n);
        for (int i = 0; i < n; i++)
            infile_test >> a[i];
        long result = SubArray::maximumSum(a, m);
        infile_rep >> s_ref;
        EXPECT_EQ(s_ref, result);
    }
}

void test_num(size_t n){
    std::cout << "Test " << n << std::endl;
    test_files("test" + std::to_string(n), "rep" + std::to_string(n));
}


TEST(basic_subarray_check, test_eq){
	EXPECT_EQ(1, 1);
}

TEST(basic_subarray_check, test_neq){
	EXPECT_NE(1, 0);
}

TEST(subarray_check, test_hr0){
    test_num(0);
}

TEST(subarray_check, test_hr1){
    test_num(1);
}

TEST(subarray_check, test_hr2){
    test_num(2);
}