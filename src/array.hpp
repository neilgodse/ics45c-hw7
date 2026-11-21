#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

template <typename T>
class Array {
public:

    Array() : len{0}, buf{nullptr} {}

    explicit Array(int len) : len{len}, buf{new T[len]} {}

    int length() const { return len; }

    ~Array() { delete[] buf; }

    T& operator[](int index) {
        if (index < 0 || index >= len) {
            throw std::string{"Exception operator[](" + std::to_string(index) + ") Out Of Range"};
        }
        return buf[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= len) {
            throw std::string{"Exception operator[](" + std::to_string(index) + ") Out Of Range"};
        }
        return buf[index];
    }
private:
    int len;
    T* buf;
};


template <typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& array) {
    std::stringstream temp;
    temp << std::setprecision(2) << std::fixed << std::right;
    for (int i = 0; i < array.length(); ++i) {
        temp << std::setw(8) << array[i];
    }
    out << temp.str();
    return out;
}

template <typename T>
std::istream& operator>>(std::istream& in, Array<T>& array) {
    for (int i = 0; i < array.length(); ++i) {
        in >> array[i];
    }
    return in;
}
#endif
