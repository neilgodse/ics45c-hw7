#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <utility>

#include "alloc.hpp"
#include "array.hpp"
#include "matrix.hpp"

using namespace std;

// ------------------------------ Array<T> ------------------------------ //

TEST(StudentArray, LengthAndDefault) {
    EXPECT_EQ(Array<int>{}.length(), 0);
    EXPECT_EQ(Array<int>{5}.length(), 5);
    EXPECT_EQ(Array<string>{3}.length(), 3);
}

TEST(StudentArray, SubscriptReadWrite) {
    Array<int> arr{4};
    for (int i = 0; i < 4; ++i) arr[i] = i * 10;
    for (int i = 0; i < 4; ++i) EXPECT_EQ(arr[i], i * 10);

    const Array<int>& cref = arr;
    EXPECT_EQ(cref[0], 0);
    EXPECT_EQ(cref[3], 30);
}

TEST(StudentArray, SubscriptOutOfRangeThrows) {
    Array<int> arr{3};
    EXPECT_THROW(arr[3], string);
    EXPECT_THROW(arr[-1], string);
    EXPECT_THROW(Array<int>{}[0], string);

    const Array<int>& cref = arr;
    EXPECT_THROW(cref[3], string);
    EXPECT_THROW(cref[-1], string);
}

TEST(StudentArray, FillSetsEveryElement) {
    Array<int> arr{6};
    arr.fill(7);
    for (int i = 0; i < arr.length(); ++i) EXPECT_EQ(arr[i], 7);

    Array<string> words{3};
    words.fill("hi");
    EXPECT_EQ(words[0], "hi");
    EXPECT_EQ(words[2], "hi");
}

TEST(StudentArray, FillWithFnUsesIndex) {
    Array<int> arr{5};
    arr.fill_with_fn([](int i) { return i * i + 1; });
    for (int i = 0; i < 5; ++i) EXPECT_EQ(arr[i], i * i + 1);
}

TEST(StudentArray, PrintFormat) {
    Array<int> arr{3};
    arr[0] = 1;
    arr[1] = -2;
    arr[2] = 30;
    stringstream out;
    out << arr;
    EXPECT_EQ(out.str(), "       1      -2      30");

    Array<double> d{2};
    d[0] = 1.5;
    d[1] = 0.125;
    stringstream out2;
    out2 << d;
    EXPECT_EQ(out2.str(), "    1.50    0.12");
}

TEST(StudentArray, ReadFillsInOrder) {
    stringstream in{"4 5 6 7"};
    Array<int> arr{3};
    in >> arr;
    EXPECT_EQ(arr[0], 4);
    EXPECT_EQ(arr[1], 5);
    EXPECT_EQ(arr[2], 6);
    int leftover;
    in >> leftover;
    EXPECT_EQ(leftover, 7);
}

TEST(StudentArray, CopyIsDeep) {
    Array<int> a{3};
    a.fill(1);
    Array<int> b{a};
    b[0] = 99;
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(b.length(), 3);
    EXPECT_EQ(b[2], 1);
}

TEST(StudentArray, CopyAssignmentIsDeepAndSelfSafe) {
    Array<int> a{2};
    a.fill(5);
    Array<int> b{7};
    b = a;
    b[0] = 42;
    EXPECT_EQ(a[0], 5);
    EXPECT_EQ(b.length(), 2);
    EXPECT_EQ(b[1], 5);

    b = b;
    EXPECT_EQ(b.length(), 2);
    EXPECT_EQ(b[0], 42);
}

TEST(StudentArray, MoveConstructorStealsBuffer) {
    AllocationTracker tracker;
    {
        Array<int> a{4};
        a.fill(3);
        size_t before = tracker.get_num_allocations();
        Array<int> b{std::move(a)};
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
        Array<int> a{4};
        a.fill(3);
        Array<int> b{2};
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
    Array<int> a{1};
    a[0] = 1;
    Array<int> b{3};
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
        Array<int> a{5};
        Array<string> s{2};
        (void)a;
        (void)s;
    }
    size_t mismatched = tracker.get_mismatching_deletes();
    size_t live = tracker.get_allocations().size();
    EXPECT_EQ(mismatched, 0u);
    EXPECT_EQ(live, 0u);
}

// ------------------------------ Matrix<T> ----------------------------- //

TEST(StudentMatrix, Dimensions) {
    Matrix<int> m{3, 5};
    EXPECT_EQ(m.num_rows(), 3);
    EXPECT_EQ(m.num_cols(), 5);
    for (int i = 0; i < 3; ++i) EXPECT_EQ(m[i].length(), 5);

    Matrix<int> empty;
    EXPECT_EQ(empty.num_rows(), 0);
    EXPECT_EQ(empty.num_cols(), 0);
}

TEST(StudentMatrix, SubscriptReadWrite) {
    Matrix<int> m{2, 3};
    m[0][2] = 8;
    m[1][0] = 4;
    EXPECT_EQ(m[0][2], 8);
    EXPECT_EQ(m[1][0], 4);

    const Matrix<int>& cref = m;
    EXPECT_EQ(cref[0][2], 8);
    EXPECT_THROW(m[2], string);
    EXPECT_THROW(m[0][3], string);
}

TEST(StudentMatrix, FillSetsEveryCell) {
    Matrix<int> m{3, 4};
    m.fill(6);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j) EXPECT_EQ(m[i][j], 6);
}

TEST(StudentMatrix, FillWithFnRowThenCol) {
    Matrix<int> m{2, 3};
    m.fill_with_fn([](int i, int j) { return i * 100 + j; });
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j) EXPECT_EQ(m[i][j], i * 100 + j);
}

TEST(StudentMatrix, PrintOneRowPerLine) {
    Matrix<int> m{2, 2};
    m.fill_with_fn([](int i, int j) { return i * 2 + j; });
    stringstream out;
    out << m;
    EXPECT_EQ(out.str(), "       0       1\n       2       3\n");
}

TEST(StudentMatrix, ReadRowMajor) {
    stringstream in{"1 2 3 4 5 6"};
    Matrix<int> m{3, 2};
    in >> m;
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[1][0], 3);
    EXPECT_EQ(m[2][1], 6);
}

TEST(StudentMatrix, CopyIsDeep) {
    Matrix<int> a{2, 2};
    a.fill(1);
    Matrix<int> b{a};
    b[0][0] = 9;
    EXPECT_EQ(a[0][0], 1);
    EXPECT_EQ(b[1][1], 1);
}

TEST(StudentMatrix, NoLeaksOrBadDeletes) {
    AllocationTracker tracker;
    {
        Matrix<int> m{4, 3};
        m.fill(2);
        Matrix<int> copy{m};
        Matrix<int> moved{std::move(copy)};
        (void)moved;
    }
    size_t live = tracker.get_allocations().size();
    size_t double_deletes = tracker.get_double_deletes();
    size_t mismatched = tracker.get_mismatching_deletes();
    EXPECT_EQ(live, 0u);
    EXPECT_EQ(double_deletes, 0u);
    EXPECT_EQ(mismatched, 0u);
}
