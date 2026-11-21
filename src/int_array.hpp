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

    int& operator[](int index) {
        if (index < 0 || index >= len) {
            throw std::string{"Exception operator[](" + std::to_string(index) + ") Out Of Range"};
        }
        return buf[index];
    }

    const int& operator[](int index) const {
        if (index < 0 || index >= len) {
            throw std::string{"Exception operator[](" + std::to_string(index) + ") Out Of Range"};
        }
        return buf[index];
    }
private:
    int len;
    int* buf;
};


inline std::ostream& operator<<(std::ostream& out, const Array& array) {
    std::stringstream temp;
    temp << std::setprecision(2) << std::fixed << std::right;
    for (int i = 0; i < array.length(); ++i) {
        temp << std::setw(8) << array[i];
    }
    out << temp.str();
    return out;
}

inline std::istream& operator>>(std::istream& in, Array& array) {
    for (int i = 0; i < array.length(); ++i) {
        in >> array[i];
    }
    return in;
}
#endif
