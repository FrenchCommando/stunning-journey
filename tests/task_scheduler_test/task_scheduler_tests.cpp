//
// Created by frenchcommando on 22/06/2019.
//

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
