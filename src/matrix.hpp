#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "array.hpp"

template <typename T>
class Matrix {
public:

private:
    int rows, cols;
    Array<Array<T>> data;
};

#endif
