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

TEST(ArrayTests, Subscript) {
    Array<int> arr{10};
    arr[0] = 3;
    arr[3] = 5;
    EXPECT_EQ(arr[0], 3);
    EXPECT_EQ(arr[3], 5);

    Array<string> words{2};
    words[0] = "hello";
    words[1] = "world";
    const Array<string>& cref = words;
    EXPECT_EQ(cref[0], "hello");
    EXPECT_EQ(cref[1], "world");
}

TEST(ArrayTests, OutOfBounds) {
    Array<int> arr{10};
    EXPECT_ANY_THROW(arr[11]);
    EXPECT_ANY_THROW(arr[10]);
    EXPECT_ANY_THROW(arr[-1]);

    const Array<int>& cref = arr;
    EXPECT_ANY_THROW(cref[10]);
    EXPECT_ANY_THROW(cref[-1]);
}

TEST(ArrayTests, Print) {
    stringstream out;
    Array<int> arr{3};
    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 2;
    out << arr;
    EXPECT_EQ(out.str(), "       0       1       2");

    stringstream out2;
    Array<double> darr{2};
    darr[0] = 1.5;
    darr[1] = -2.25;
    out2 << darr;
    EXPECT_EQ(out2.str(), "    1.50   -2.25");
}

TEST(ArrayTests, Read) {
    stringstream in{"1 2 3 4"};
    Array<int> arr{3};
    in >> arr;
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);

    stringstream in2{"foo bar"};
    Array<string> words{2};
    in2 >> words;
    EXPECT_EQ(words[0], "foo");
    EXPECT_EQ(words[1], "bar");
}

TEST(ArrayTests, Fill) {
    Array<int> arr{10};
    arr.fill(20);
    EXPECT_EQ(arr[0], 20);
    arr.fill(5);
    EXPECT_EQ(arr[9], 5);

    Array<string> words{3};
    words.fill("hi");
    EXPECT_EQ(words[0], "hi");
    EXPECT_EQ(words[2], "hi");
}

TEST(ArrayTests, FillWithFn) {
    Array<int> arr{5};
    arr.fill_with_fn([](int i) { return i * i; });
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[2], 4);
    EXPECT_EQ(arr[4], 16);

    Array<string> words{3};
    words.fill_with_fn([](int i) { return string(i, 'x'); });
    EXPECT_EQ(words[0], "");
    EXPECT_EQ(words[2], "xx");
}

TEST(ArrayTests, CopyConstructor) {
    Array<int> arr{10};
    arr.fill(20);
    Array<int> arr2{arr};
    EXPECT_EQ(arr2.length(), 10);
    EXPECT_EQ(arr2[0], 20);
    EXPECT_EQ(arr2[9], 20);
    arr[0] = 1;
    EXPECT_EQ(arr2[0], 20);
    Array<int> empty;
    Array<int> arr3{empty};
    EXPECT_EQ(arr3.length(), 0);
}

TEST(ArrayTests, MoveConstructor) {
    AllocationTracker tracker;
    Array<int> arr{10};
    arr.fill(20);
    std::size_t allocs_before_move = tracker.get_num_allocations();
    Array<int> arr2{std::move(arr)};
    std::size_t allocs_after_move = tracker.get_num_allocations();
    EXPECT_EQ(allocs_before_move, allocs_after_move);
    EXPECT_EQ(arr2.length(), 10);
    EXPECT_EQ(arr2[0], 20);
    EXPECT_EQ(arr2[9], 20);
    Array<int> empty;
    Array<int> arr3{std::move(empty)};
    EXPECT_EQ(arr3.length(), 0);
}

TEST(ArrayTests, Swap) {
    AllocationTracker tracker;
    Array<int> arr{10};
    arr.fill(20);
    Array<int> arr2{2};
    arr2.fill(5);
    std::size_t allocs_before_swap = tracker.get_num_allocations();
    swap(arr, arr2);
    std::size_t allocs_after_swap = tracker.get_num_allocations();
    EXPECT_EQ(allocs_before_swap, allocs_after_swap);
    EXPECT_EQ(arr2.length(), 10);
    EXPECT_EQ(arr2[0], 20);
    EXPECT_EQ(arr2[9], 20);
    EXPECT_EQ(arr.length(), 2);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 5);
}

TEST(ArrayTests, CopyAssignment) {
    Array<int> arr{10};
    arr.fill(20);
    Array<int> arr2{2};
    arr2.fill(5);
    arr = arr2;
    EXPECT_EQ(arr.length(), 2);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 5);
    EXPECT_EQ(arr2.length(), 2);
    EXPECT_EQ(arr2[0], 5);
    EXPECT_EQ(arr2[1], 5);
    arr = arr;
    EXPECT_EQ(arr.length(), 2);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 5);
}

TEST(ArrayTests, MoveAssignment) {
    AllocationTracker tracker;
    Array<int> arr{10};
    arr.fill(20);
    Array<int> arr2{2};
    arr2.fill(5);
    std::size_t allocs_before_move = tracker.get_num_allocations();
    arr = std::move(arr2);
    std::size_t allocs_after_move = tracker.get_num_allocations();
    EXPECT_EQ(allocs_before_move, allocs_after_move);
    EXPECT_EQ(arr.length(), 2);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 5);
    arr2 = Array<int>{5};
    EXPECT_EQ(arr2.length(), 5);
    arr2 = std::move(arr2);
    arr2 = Array<int>{7};
    EXPECT_EQ(arr2.length(), 7);
}
