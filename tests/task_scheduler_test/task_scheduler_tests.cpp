//
// Created by frenchcommando on 22/06/2019.
//

#include <fstream>
#include "task_scheduler/TaskScheduler.h"
#include "gtest/gtest.h"


TEST(task_check, test_duplicate){
    TaskScheduler t;
    int i = 30;
    int j = 2;
    auto r0 = t.add_task(i, j);
    EXPECT_EQ(0, r0);
    auto r1 = t.add_task(i, j);
    EXPECT_EQ(0, r1);
} // not reading some out of bound value

TEST(task_check, test_one){
    TaskScheduler t;
    auto r0 = t.add_task(1, 1);
    EXPECT_EQ(0, r0);
    auto r1 = t.add_task(1, 1);
    EXPECT_EQ(1, r1);
    auto r2 = t.add_task(1, 1);
    EXPECT_EQ(2, r2);
}

TEST(task_check, test_big_d){
    TaskScheduler t;
    int d = 100;
    EXPECT_EQ(0, t.add_task(d,1));
    EXPECT_EQ(0, t.add_task(d,1));
    EXPECT_EQ(0, t.add_task(d,1));
}

TEST(task_check, test_big_m){
    TaskScheduler t;
    int m = 100;
    EXPECT_EQ(m - 1, t.add_task(1,m));
    EXPECT_EQ(2 * m - 1, t.add_task(1,m));
    EXPECT_EQ(3 * m - 1, t.add_task(1,m));
}

TEST(task_check, test_buffer){
    TaskScheduler t;
    int m = 100;
    EXPECT_EQ(m - 1, t.add_task(1,m));
    EXPECT_EQ(m, t.add_task(1,1));
    EXPECT_EQ(m + 1, t.add_task(1,1));
}

void test_files(const std::string& f1, const std::string& f2){
    std::string dir = "/mnt/c/Users/marti/CLionProjects/stunning-journey/tests/task_scheduler_test/hr_txt/";
    std::ifstream infile_test (dir + f1);
    std::ifstream infile_rep (dir + f2);
    int n;
    int d, m, s_ref;
    infile_test >> n;
    TaskScheduler t;
    while (infile_test >> d){
        infile_test >> m;
        infile_rep >> s_ref;
        EXPECT_EQ(s_ref, t.add_task(d, m));
    }
}

void test_num(size_t n){
    std::cout << "Test " << n << std::endl;
    test_files("test" + std::to_string(n), "rep" + std::to_string(n));
}

TEST(task_check, test_hr1){
    test_num(1);
}
