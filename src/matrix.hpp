#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "array.hpp"

template <typename T>
class Matrix {
public:

    Matrix() : rows{0}, cols{0}, data{0} {}

    Matrix(int rows, int cols) : rows{rows}, cols{cols}, data{rows} {
        for (int i = 0; i < rows; ++i) {
            data[i] = Array<T>{cols};
        }
    }

    int num_rows() const { return rows; }

    int num_cols() const { return cols; }
private:
    int rows, cols;
    Array<Array<T>> data;
};

#endif
