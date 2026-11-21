#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "alloc.hpp"
#include "array.hpp"

using namespace std;

TEST(ArrayTests, DefaultConstructor) {
    AllocationTracker tracker;
    {
        Array<int> a;
        Array<string> b;
        (void)a;
        (void)b;
    }
    std::size_t allocs = tracker.get_num_allocations();
    EXPECT_EQ(allocs, 0u);
}

TEST(ArrayTests, SizedConstructorAndDestructor) {
    AllocationTracker tracker;
    {
        Array<int> a{10};
        Array<double> b{4};
        (void)a;
        (void)b;
    }
    std::size_t live = tracker.get_allocations().size();
    EXPECT_EQ(live, 0u);
}

TEST(ArrayTests, Length) {
    Array<int> arr{3};
    EXPECT_EQ(arr.length(), 3);

    Array<string> arr2{10};
    EXPECT_EQ(arr2.length(), 10);

    EXPECT_EQ(Array<double>{}.length(), 0);
}
