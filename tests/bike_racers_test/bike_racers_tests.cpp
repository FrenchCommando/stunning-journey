//
// Created by frenchcommando on 2019-07-14.
//
#include <fstream>
#include "bike_racers/BikeRacers.h"
#include "gtest/gtest.h"


TEST(basic_bike_racers_check, test_eq){
	EXPECT_EQ(1, 1);
}

TEST(basic_bike_racers_check, test_neq){
	EXPECT_NE(1, 0);
}

void test_files(const std::string& f1, const std::string& f2){
    std::string dir = "/mnt/c/Users/marti/CLionProjects/stunning-journey/tests/bike_racers_test/hr_txt/";
    std::ifstream infile_test (dir + f1);
    std::ifstream infile_rep (dir + f2);
    int n, m, k;
    infile_test >> n >> m >> k;
    std::vector<std::vector<int>> bikers;
    int x, y;
    for (size_t i = 0; i < n; i++){
        infile_test >> x >> y;
        bikers.emplace_back(std::vector<int>{x, y});
    }

    std::vector<std::vector<int>> bikes;
    for (size_t i = 0; i < m; i++){
        infile_test >> x >> y;
        bikes.emplace_back(std::vector<int>{x, y});
    }
    long rep;
    infile_rep >> rep;
    EXPECT_EQ(rep, BikeRacers::solve(bikers, bikes, k));
}

void test_num(size_t n){
    std::cout << "Test " << n << std::endl;
    test_files("test" + std::to_string(n), "rep" + std::to_string(n));
}

TEST(basic_bike_racers_check, test_hr0){
    test_num(0);
}

TEST(basic_bike_racers_check, test_hr1){
    test_num(1);
}

TEST(basic_bike_racers_check, test_hr14){
    test_num(14);
}
