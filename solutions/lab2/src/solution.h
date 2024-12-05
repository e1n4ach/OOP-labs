#pragma once

#include <initializer_list>
#include <iostream>
#include <string>

class Eleven {
public:
    Eleven();
    Eleven(const size_t &n, unsigned char t = '0');
    Eleven(const std::initializer_list<unsigned char> &lst);
    Eleven(const std::string &str);
    Eleven(const Eleven &other);
    Eleven(Eleven &&other) noexcept;
    ~Eleven() noexcept;

    Eleven &operator=(const Eleven &other);
    Eleven &operator=(Eleven &&other) noexcept;
    Eleven &operator+=(const Eleven &other);
    Eleven &operator-=(const Eleven &other);
    friend Eleven operator+(const Eleven &a, const Eleven &b);
    friend Eleven operator-(const Eleven &a, const Eleven &b);

    friend bool operator==(const Eleven &a, const Eleven &b);
    friend bool operator!=(const Eleven &a, const Eleven &b);
    friend bool operator<(const Eleven &a, const Eleven &b);
    friend bool operator>(const Eleven &a, const Eleven &b);
    friend bool operator<=(const Eleven &a, const Eleven &b);
    friend bool operator>=(const Eleven &a, const Eleven &b);

    friend std::ostream &operator<<(std::ostream &os, const Eleven &num);

private:
    bool isInvalidDigit(unsigned char c);
    bool isValidElevenString(const std::string &str);
    bool isValidElevenInitList(const std::initializer_list<unsigned char> &lst);

private:
    size_t _sz;
    unsigned char *_arr;
};
