#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "alloc.hpp"
#include "matrix.hpp"

using namespace std;

TEST(MatrixTests, Empty) {
    AllocationTracker tracker;
    {
        Matrix<int> mat;
        Matrix<string> mat2;
        (void)mat;
        (void)mat2;
    }
    std::size_t live = tracker.get_allocations().size();
    EXPECT_EQ(live, 0u);
}

TEST(MatrixTests, Constructor) {
    AllocationTracker tracker;
    {
        Matrix<int> mat{3, 2};
        Matrix<string> mat2{2, 4};
        (void)mat;
        (void)mat2;
    }
    std::size_t live = tracker.get_allocations().size();
    EXPECT_EQ(live, 0u);
}

TEST(MatrixTests, NumRows) {
    EXPECT_EQ((Matrix<int>{3, 2}.num_rows()), 3);
    EXPECT_EQ((Matrix<double>{5, 1}.num_rows()), 5);
    EXPECT_EQ(Matrix<string>{}.num_rows(), 0);
}

TEST(MatrixTests, RowsAndCols) {
    Matrix<int> mat{3, 2};
    EXPECT_EQ(mat.num_rows(), 3);
    EXPECT_EQ(mat.num_cols(), 2);

    Matrix<double> mat2{5, 1};
    EXPECT_EQ(mat2.num_rows(), 5);
    EXPECT_EQ(mat2.num_cols(), 1);

    Matrix<string> mat3;
    EXPECT_EQ(mat3.num_rows(), 0);
    EXPECT_EQ(mat3.num_cols(), 0);
}
