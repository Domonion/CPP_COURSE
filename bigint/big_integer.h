#ifndef big_integer_H
#define big_integer_H
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>

class big_integer {

private:
    bool minus;
    std::vector<unsigned int> num;

    void check();

public:

    const std::vector<unsigned int>& getNum() const {
        return num;
    }

    big_integer(std::vector<unsigned int> const &number, bool sign);

    big_integer();

    big_integer(big_integer const &other) = default;

    big_integer(int a);

    explicit big_integer(std::string s);

    ~big_integer() = default;

    big_integer inverse() const;

    big_integer(unsigned long long a);

    big_integer(unsigned int a);

    big_integer &operator=(big_integer const &other) = default;

    big_integer &operator+=(big_integer const &other);

    big_integer &operator-=(big_integer const &other);

    big_integer &operator/=(big_integer const &other);

    big_integer &operator%=(big_integer const &other);

    big_integer &operator*=(big_integer const &other);

    big_integer &operator&=(big_integer const &other);

    big_integer &operator|=(big_integer const &other);

    big_integer &operator^=(big_integer const &other);

    big_integer &operator<<=(unsigned int other);

    big_integer &operator>>=(unsigned int other);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~() const;

    big_integer &operator++();

    big_integer operator++(int);

    big_integer &operator--();

    big_integer operator--(int);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend std::string to_string(big_integer const &a);

    friend std::ostream &operator<<(std::ostream &s, big_integer const &a);

    friend big_integer operator+(big_integer a, big_integer const &b);

    friend big_integer operator-(big_integer a, big_integer const &b);

    friend big_integer operator*(big_integer, big_integer const &b);

    friend std::vector<unsigned int> divU(std::vector<unsigned int> const &, unsigned int b);

    friend big_integer operator/(big_integer, big_integer const &b);

    friend big_integer operator%(big_integer, big_integer const &b);

    friend big_integer operator&(big_integer, big_integer const &b);

    friend big_integer operator|(big_integer, big_integer const &b);

    friend big_integer operator^(big_integer, big_integer const &b);

    friend big_integer operator<<(big_integer, int b);

    friend big_integer operator>>(big_integer, int b);
};

void fastdiv(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b, std::vector<unsigned int> & c);

bool isLess(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b);

void add(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b, std::vector<unsigned int> & res);

void sub(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b, std::vector<unsigned int> & res);

void mul(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b, std::vector<unsigned int> & res);

void and_long(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b, std::vector<unsigned int> & res);

void or_long(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b, std::vector<unsigned int> & res);

void xor_long(std::vector<unsigned int> const &a, std::vector<unsigned int> const &b, std::vector<unsigned int> & res);

void shift_left(std::vector<unsigned int> const &a, int b, std::vector<unsigned int> & res);

void shift_right(std::vector<unsigned int> const &a, int b, std::vector<unsigned int> & res);

std::vector<unsigned int> & correct(std::vector<unsigned int> & a);

#endif //big_integer_H
