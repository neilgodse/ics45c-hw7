#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <utility>

#include "alloc.hpp"
#include "int_array.hpp"

using namespace std;

TEST(StudentArray, LengthAndDefault) {
    EXPECT_EQ(Array{}.length(), 0);
    EXPECT_EQ(Array{5}.length(), 5);
}

TEST(StudentArray, SubscriptReadWrite) {
    Array arr{4};
    for (int i = 0; i < 4; ++i) arr[i] = i * 10;
    for (int i = 0; i < 4; ++i) EXPECT_EQ(arr[i], i * 10);

    const Array& cref = arr;
    EXPECT_EQ(cref[0], 0);
    EXPECT_EQ(cref[3], 30);
}

TEST(StudentArray, SubscriptOutOfRangeThrows) {
    Array arr{3};
    EXPECT_THROW(arr[3], string);
    EXPECT_THROW(arr[-1], string);
    EXPECT_THROW(Array{}[0], string);

    const Array& cref = arr;
    EXPECT_THROW(cref[3], string);
    EXPECT_THROW(cref[-1], string);
}

TEST(StudentArray, ConstSubscriptThrowsOnDefault) {
    const Array empty;
    EXPECT_THROW(empty[0], string);
}

TEST(StudentArray, OutOfRangeMessageNamesIndex) {
    Array arr{2};
    try {
        arr[5];
        FAIL() << "expected throw";
    } catch (const string& e) {
        EXPECT_NE(e.find("5"), string::npos);
    }
    const Array& cref = arr;
    try {
        cref[-3];
        FAIL() << "expected throw";
    } catch (const string& e) {
        EXPECT_NE(e.find("-3"), string::npos);
    }
}

TEST(StudentArray, FillSetsEveryElement) {
    Array arr{6};
    arr.fill(7);
    for (int i = 0; i < arr.length(); ++i) EXPECT_EQ(arr[i], 7);
}

TEST(StudentArray, PrintFormat) {
    Array arr{3};
    arr[0] = 1;
    arr[1] = -2;
    arr[2] = 30;
    stringstream out;
    out << arr;
    EXPECT_EQ(out.str(), "       1      -2      30");

    stringstream empty_out;
    empty_out << Array{};
    EXPECT_EQ(empty_out.str(), "");
}

TEST(StudentArray, ReadFillsInOrder) {
    stringstream in{"4 5 6 7"};
    Array arr{3};
    in >> arr;
    EXPECT_EQ(arr[0], 4);
    EXPECT_EQ(arr[1], 5);
    EXPECT_EQ(arr[2], 6);
    int leftover;
    in >> leftover;
    EXPECT_EQ(leftover, 7);
}

TEST(StudentArray, CopyIsDeep) {
    Array a{3};
    a.fill(1);
    Array b{a};
    b[0] = 99;
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(b.length(), 3);
    EXPECT_EQ(b[2], 1);
}

TEST(StudentArray, CopyConstructorCopiesAll) {
    Array a{5};
    for (int i = 0; i < 5; ++i) a[i] = i + 1;
    Array b{a};
    for (int i = 0; i < 5; ++i) EXPECT_EQ(b[i], i + 1);
}

TEST(StudentArray, CopyAssignmentIsDeepAndSelfSafe) {
    Array a{2};
    a.fill(5);
    Array b{7};
    b = a;
    b[0] = 42;
    EXPECT_EQ(a[0], 5);
    EXPECT_EQ(b.length(), 2);
    EXPECT_EQ(b[1], 5);

    b = b;
    EXPECT_EQ(b.length(), 2);
    EXPECT_EQ(b[0], 42);
}

TEST(StudentArray, CopyAssignmentNoLeak) {
    AllocationTracker tracker;
    {
        Array a{3};
        a.fill(1);
        Array b{5};
        b = a;
        a = a;
    }
    size_t live = tracker.get_allocations().size();
    size_t double_deletes = tracker.get_double_deletes();
    EXPECT_EQ(live, 0u);
    EXPECT_EQ(double_deletes, 0u);
}

TEST(StudentArray, MoveConstructorStealsBuffer) {
    AllocationTracker tracker;
    {
        Array a{4};
        a.fill(3);
        size_t before = tracker.get_num_allocations();
        Array b{std::move(a)};
        size_t after = tracker.get_num_allocations();
        EXPECT_EQ(before, after);
        EXPECT_EQ(b.length(), 4);
        EXPECT_EQ(b[3], 3);
        EXPECT_EQ(a.length(), 0);
    }
    size_t double_deletes = tracker.get_double_deletes();
    size_t live = tracker.get_allocations().size();
    EXPECT_EQ(double_deletes, 0u);
    EXPECT_EQ(live, 0u);
}

TEST(StudentArray, MoveAssignmentNoAllocs) {
    AllocationTracker tracker;
    {
        Array a{4};
        a.fill(3);
        Array b{2};
        size_t before = tracker.get_num_allocations();
        b = std::move(a);
        size_t after = tracker.get_num_allocations();
        EXPECT_EQ(before, after);
        EXPECT_EQ(b.length(), 4);
        EXPECT_EQ(b[0], 3);
    }
    size_t double_deletes = tracker.get_double_deletes();
    size_t live = tracker.get_allocations().size();
    EXPECT_EQ(double_deletes, 0u);
    EXPECT_EQ(live, 0u);
}

TEST(StudentArray, SwapExchangesContents) {
    Array a{1};
    a[0] = 1;
    Array b{3};
    b.fill(2);
    swap(a, b);
    EXPECT_EQ(a.length(), 3);
    EXPECT_EQ(a[2], 2);
    EXPECT_EQ(b.length(), 1);
    EXPECT_EQ(b[0], 1);
}

TEST(StudentArray, DestructorUsesArrayDelete) {
    AllocationTracker tracker;
    {
        Array a{5};
        (void)a;
    }
    size_t mismatched = tracker.get_mismatching_deletes();
    size_t live = tracker.get_allocations().size();
    EXPECT_EQ(mismatched, 0u);
    EXPECT_EQ(live, 0u);
}
