//
// Created by Davide Russo on 15/07/18.
//

#include <functional.hpp>
#include "gtest/gtest.h"

using namespace functional;

class FunctionalTest : public ::testing::Test {
protected:
    void TearDown() override {}

    void SetUp() override {}

public:
    FunctionalTest() : Test() {
        p_func_list = new functional_vector<int>{1, 2, 10, 15, -2, -8, 15};
    }

    ~FunctionalTest() override {
        delete p_func_list;
    }

    functional_vector<int> *p_func_list = nullptr;
};

TEST_F(FunctionalTest, test_filter) {
    auto count = p_func_list->filter([](auto x) { return x < 0; }).size();
    EXPECT_EQ(2, count);
}

TEST_F(FunctionalTest, test_reduce) {
    auto x = p_func_list->reduce(0, [](auto acc, auto x) { return acc + x; });
    EXPECT_EQ(x, 33);
}
