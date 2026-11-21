#ifndef INT_ARRAY_HPP
#define INT_ARRAY_HPP

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

class Array {
public:

    Array() : len{0}, buf{nullptr} {}

    explicit Array(int len) : len{len}, buf{new int[len]} {}

    int length() const { return len; }

    ~Array() { delete[] buf; }
private:
    int len;
    int* buf;
};

#endif
