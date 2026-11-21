#include <gtest/gtest.h>

#include <sstream>

#include "alloc.hpp"
#include "int_array.hpp"

using namespace std;

TEST(IntArrayTests, DefaultConstructor) {
    AllocationTracker tracker;
    {
        Array arr;
        (void)arr;
    }
    std::size_t allocs = tracker.get_num_allocations();
    EXPECT_EQ(allocs, 0u);
}

TEST(IntArrayTests, SizedConstructorAndDestructor) {
    AllocationTracker tracker;
    {
        Array arr{10};
        (void)arr;
    }
    std::size_t allocs = tracker.get_num_allocations();
    std::size_t live = tracker.get_allocations().size();
    EXPECT_EQ(allocs, 1u);
    EXPECT_EQ(live, 0u);
}

TEST(IntArrayTests, Length) {
    Array arr{10};
    EXPECT_EQ(arr.length(), 10);
    EXPECT_EQ(Array{}.length(), 0);
}
