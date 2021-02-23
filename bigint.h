#ifndef BIGINT_H
#define BIGINT_H

#include <type_traits>
#include <iostream>

#include "ubigint.h"

class BigInt {
public:
    BigInt() = default;
    BigInt(char rhs, bool negative=0): magnitude{rhs}, neg{negative} {}
    BigInt(std::string rhs);
    BigInt(UBigInt rhs, bool negative=0) : magnitude{rhs}, neg{negative} {}
    template <class T,
              typename std::enable_if<std::is_integral<T>::value, int>::type* = nullptr>
    BigInt(T rhs): magnitude{rhs} {
        if (rhs < 0) {
            neg = true;
        }
        else {
            neg = false;
        }
    }
    template <typename Iter>
    BigInt(Iter begin, Iter end, bool negative=0) : magnitude{begin, end},
                                                    neg{negative} {}
    BigInt(const BigInt &rhs) = default;
    BigInt(BigInt &&rhs) = default;
    BigInt& operator=(const BigInt &rhs) = default;
    BigInt& operator=(BigInt &&rhs) = default;
    ~BigInt() = default;

    BigInt& operator+=(const BigInt &rhs);
    BigInt& operator-=(const BigInt &rhs);
    BigInt& operator*=(const BigInt &rhs);
    BigInt operator/=(const BigInt &rhs);
    BigInt& operator++();
    BigInt& operator--();
    BigInt operator++(int);
    BigInt operator--(int);
    friend BigInt operator+(const BigInt &lhs, const BigInt &rhs);
    friend BigInt operator-(const BigInt &lhs, const BigInt &rhs);
    friend BigInt operator*(const BigInt &lhs, const BigInt &rhs);
    friend BigInt operator/(const BigInt &lhs, const BigInt &rhs);
    friend bool operator>(const BigInt &lhs, const BigInt &rhs);
    friend bool operator<(const BigInt &lhs, const BigInt &rhs);
    friend bool operator==(const BigInt &lhs, const BigInt &rhs);
    friend bool operator!=(const BigInt &lhs, const BigInt &rhs);
    friend bool operator>=(const BigInt &lhs, const BigInt &rhs);
    friend bool operator<=(const BigInt &lhs, const BigInt &rhs);
    friend std::ostream& operator<<(std::ostream &out, const BigInt &rhs);
    BigInt& randomize(const size_t &length);
    BigInt& power(const BigInt &rhs);
    BigInt& shift10(int m=1);
    BigInt& abs() {neg = false;}
    size_t get_length() {return magnitude.get_length();}
    uint karatsuba_thres = 64;
private:
    UBigInt magnitude;
    bool neg = false;
    BigInt karatsuba(BigInt lhs, BigInt rhs);
};

bool test_arithmatic();
void test_util();
void find_karatsuba_thres(std::pair<int, int> k_test_limits={16,128},
                          int k_test_increment=8,
                          int n_per_increment=1,
                          size_t size_number=1000);

#endif
