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

TEST(MatrixTests, Subscript) {
    Matrix<int> mat{2, 3};
    mat[0][0] = 1;
    mat[1][2] = 7;
    EXPECT_EQ(mat[0][0], 1);
    EXPECT_EQ(mat[1][2], 7);
    EXPECT_EQ(mat[0].length(), 3);

    const Matrix<int>& cref = mat;
    EXPECT_EQ(cref[1][2], 7);
    EXPECT_ANY_THROW(mat[0][3]);
    EXPECT_ANY_THROW(mat[2]);
}

TEST(MatrixTests, Fill) {
    Matrix<int> mat{3, 4};
    mat.fill(9);
    EXPECT_EQ(mat[0][0], 9);
    EXPECT_EQ(mat[2][3], 9);

    Matrix<string> words{2, 2};
    words.fill("ok");
    EXPECT_EQ(words[1][1], "ok");
}

TEST(MatrixTests, FillWithFn) {
    Matrix<int> mat{3, 3};
    mat.fill_with_fn([](int i, int j) { return i * 10 + j; });
    EXPECT_EQ(mat[0][0], 0);
    EXPECT_EQ(mat[1][2], 12);
    EXPECT_EQ(mat[2][1], 21);
}

TEST(MatrixTests, Print) {
    Matrix<int> mat{2, 2};
    mat[0][0] = 1;
    mat[0][1] = 2;
    mat[1][0] = 3;
    mat[1][1] = 4;
    stringstream out;
    out << mat;
    EXPECT_EQ(out.str(), "       1       2\n       3       4\n");
}

TEST(MatrixTests, Read) {
    stringstream in{"1 2 3\n4 5 6"};
    Matrix<int> mat{2, 3};
    in >> mat;
    EXPECT_EQ(mat[0][0], 1);
    EXPECT_EQ(mat[0][2], 3);
    EXPECT_EQ(mat[1][0], 4);
    EXPECT_EQ(mat[1][2], 6);
}
