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

TEST(IntArrayTests, Subscript) {
    Array arr{10};
    arr[0] = 3;
    arr[3] = 5;
    EXPECT_EQ(arr[0], 3);
    EXPECT_EQ(arr[3], 5);

    const Array& cref = arr;
    EXPECT_EQ(cref[0], 3);
    EXPECT_EQ(cref[3], 5);
}

TEST(IntArrayTests, OutOfBounds) {
    Array arr{10};
    EXPECT_ANY_THROW(arr[11]);
    EXPECT_ANY_THROW(arr[10]);
    EXPECT_ANY_THROW(arr[-1]);

    const Array& cref = arr;
    EXPECT_ANY_THROW(cref[10]);
    EXPECT_ANY_THROW(cref[-1]);
}
