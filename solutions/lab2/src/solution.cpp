#include "solution.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>

int digitToValue(unsigned char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c == 'A') {
        return 10;
    } else {
        throw std::invalid_argument("Invalid digit in base 11 number.");
    }
}

unsigned char valueToDigit(int val) {
    if (val >= 0 && val <= 9) {
        return '0' + val;
    } else if (val == 10) {
        return 'A';
    } else {
        throw std::invalid_argument("Invalid value for base 11 digit.");
    }
}

bool Eleven::isInvalidDigit(unsigned char c) {
    return !((c >= '0' && c <= '9') || c == 'A'); 
}

bool Eleven::isValidElevenString(const std::string &str) {
    for (unsigned char c : str) {
        if (isInvalidDigit(c)) {
            return false;
        }
    }
    return true;
}

bool Eleven::isValidElevenInitList(const std::initializer_list<unsigned char> &lst) {
    for (unsigned char c : lst) {
        if (isInvalidDigit(c)) {
            return false;
        }
    }
    return true;
}

Eleven::Eleven() : _sz(0), _arr(nullptr) {}

Eleven::Eleven(const size_t &n, unsigned char t) : _sz(n), _arr(new unsigned char[n]) {
    if (isInvalidDigit(t)) {
        throw std::invalid_argument("Invalid digit for Eleven initialization.");
    }
    std::fill(this->_arr, this->_arr + _sz, t);
}

Eleven::Eleven(const std::initializer_list<unsigned char> &lst) {
    if (!isValidElevenInitList(lst)) {
        throw std::invalid_argument("Invalid initializer_list for Eleven initialization.");
    }
    _sz = lst.size();
    _arr = new unsigned char[_sz];
    std::copy(lst.begin(), lst.end(), _arr);
}

Eleven::Eleven(const std::string &str) {
    if (!isValidElevenString(str)) {
        throw std::invalid_argument("Invalid string for Eleven initialization.");
    }
    _sz = str.size();
    _arr = new unsigned char[_sz];
    std::copy(str.rbegin(), str.rend(), _arr); 
}

Eleven::Eleven(const Eleven &other) : _sz(other._sz), _arr(new unsigned char[other._sz]) {
    std::copy(other._arr, other._arr + _sz, _arr);
}

Eleven::Eleven(Eleven &&other) noexcept : _sz(other._sz), _arr(other._arr) {
    other._sz = 0;
    other._arr = nullptr;
}

Eleven::~Eleven() noexcept { delete[] _arr; }

Eleven &Eleven::operator=(const Eleven &other) {
    if (this != &other) {
        delete[] _arr;
        _sz = other._sz;
        _arr = new unsigned char[_sz];
        std::copy(other._arr, other._arr + _sz, _arr);
    }
    return *this;
}

Eleven &Eleven::operator=(Eleven &&other) noexcept {
    if (this != &other) {
        delete[] _arr;
        _sz = other._sz;
        _arr = other._arr;
        other._sz = 0;
        other._arr = nullptr;
    }
    return *this;
}

Eleven &Eleven::operator+=(const Eleven &other) {
    size_t max_sz = std::max(_sz, other._sz) + 1;
    int *temp = new int[max_sz]();

    for (size_t i = 0; i < _sz; ++i) {
        temp[i] += digitToValue(_arr[i]);
    }
    for (size_t i = 0; i < other._sz; ++i) {
        temp[i] += digitToValue(other._arr[i]);
    }

    for (size_t i = 0; i < max_sz - 1; ++i) {
        if (temp[i] >= 11) {
            temp[i + 1] += temp[i] / 11;
            temp[i] %= 11;
        }
    }

    while (max_sz > 1 && temp[max_sz - 1] == 0) {
        --max_sz;
    }

    delete[] _arr;
    _sz = max_sz;
    _arr = new unsigned char[_sz];
    for (size_t i = 0; i < _sz; ++i) {
        _arr[i] = valueToDigit(temp[i]);
    }

    delete[] temp;
    return *this;
}

Eleven &Eleven::operator-=(const Eleven &other) {
    if (*this < other) {
        throw std::invalid_argument("Subtraction result is negative.");
    }

    size_t max_sz = _sz;
    int *temp = new int[max_sz]();

    for (size_t i = 0; i < _sz; ++i) {
        temp[i] = digitToValue(_arr[i]);
    }
    for (size_t i = 0; i < other._sz; ++i) {
        temp[i] -= digitToValue(other._arr[i]);
    }

    for (size_t i = 0; i < max_sz - 1; ++i) {
        if (temp[i] < 0) {
            temp[i] += 11;
            temp[i + 1]--;
        }
    }

    while (max_sz > 1 && temp[max_sz - 1] == 0) {
        --max_sz;
    }

    delete[] _arr;
    _sz = max_sz;
    _arr = new unsigned char[_sz];
    for (size_t i = 0; i < _sz; ++i) {
        _arr[i] = valueToDigit(temp[i]);
    }

    delete[] temp;
    return *this;
}

Eleven operator+(const Eleven &a, const Eleven &b) {
    Eleven temp(a);
    temp += b;
    return temp;
}

Eleven operator-(const Eleven &a, const Eleven &b) {
    Eleven temp(a);
    temp -= b;
    return temp;
}

bool operator<(const Eleven &a, const Eleven &b) {
    if (a._sz != b._sz) {
        return a._sz < b._sz;
    }
    for (int i = a._sz - 1; i >= 0; --i) {
        int digitA = digitToValue(a._arr[i]);
        int digitB = digitToValue(b._arr[i]);
        if (digitA != digitB) {
            return digitA < digitB;
        }
    }
    return false;
}

bool operator>(const Eleven &a, const Eleven &b) { return b < a; }

bool operator==(const Eleven &a, const Eleven &b) {
    if (a._sz != b._sz) return false;
    for (size_t i = 0; i < a._sz; ++i) {
        if (a._arr[i] != b._arr[i]) return false;
    }
    return true;
}

bool operator!=(const Eleven &a, const Eleven &b) {
    return !(a == b);
}

bool operator<=(const Eleven &a, const Eleven &b) {
    return !(b < a);
}

bool operator>=(const Eleven &a, const Eleven &b) {
    return !(a < b);
}

std::ostream &operator<<(std::ostream &os, const Eleven &num) {
    for (int i = num._sz - 1; i >= 0; --i) {
        os << num._arr[i];
    }
    return os;
}