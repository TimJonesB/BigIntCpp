#ifndef BIGINT_H
#define BIGINT_H

#include <type_traits>
#include <iostream>
#include <cmath>
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
    BigInt& abs();
    size_t get_length() {return magnitude.get_length();}
    BigInt& set_karatsuba_thres(size_t thres); 
    size_t get_karatsuba_thres() {return this->karatsuba_thres;}
private:
    UBigInt magnitude;
    bool neg = false;
    size_t karatsuba_thres = 48;
    BigInt karatsuba(BigInt lhs, BigInt rhs);
};


BigInt::BigInt(std::string s) {
    neg = false;
    int i = 0;
    while (s[i] == ' ') {
        i++;
    }
    if (s[i] == '-' || s[i] == '+') {
        if (s[i] == '-') {
            neg = true;
        }
        i++;
    }
    magnitude = s.substr(i, s.size());
    if (magnitude == 0) {
        neg = false;
    }
}


std::ostream& operator<<(std::ostream &out, const BigInt &rhs) {
    if (rhs.neg) {
        std::cout << '-';
    }
    std::cout << rhs.magnitude; 
    return out;
}


BigInt& BigInt::operator+=(const BigInt &rhs) {
    if (neg && !rhs.neg) {
        if (magnitude == rhs.magnitude) {
            magnitude = {0};
            neg = false;
        }
        else if (magnitude < rhs.magnitude) {
            neg = false;
            magnitude = rhs.magnitude - magnitude;
        }
        else if (magnitude > rhs.magnitude) {
            neg = true;
            magnitude -= rhs.magnitude;
        }
    }

    else if (!neg && rhs.neg) {
        if (magnitude == rhs.magnitude) {
            magnitude = {0};
            neg = false;
        }
        else if (magnitude < rhs.magnitude) {
            neg = true;
            magnitude = rhs.magnitude - magnitude;
        }
        else if (magnitude > rhs.magnitude) {
            neg = false;
            magnitude -= rhs.magnitude;
        }
    }

    else if(!neg && !rhs.neg) {
        neg = false;
        magnitude += rhs.magnitude;
    }

    else {
        neg = true;
        magnitude += rhs.magnitude;
    }

    return *this;
}


BigInt& BigInt::operator-=(const BigInt &rhs) {
    if (neg && !rhs.neg) {
        neg = true;
        magnitude += rhs.magnitude;
    }
    else if (!neg && rhs.neg) {
        neg = false;
        magnitude += rhs.magnitude;
    }
    else if(!neg && !rhs.neg) {
        if (magnitude == rhs.magnitude) {
            magnitude = {0};
            neg = false;
        }
        else if (magnitude < rhs.magnitude) {
            neg = true;
            magnitude = rhs.magnitude - magnitude;
        }
        else if (magnitude > rhs.magnitude) {
            neg = false;
            magnitude -= rhs.magnitude;
        }
    }
    else {
        if (magnitude == rhs.magnitude) {
            magnitude = {0};
            neg = false;
        }
        else if (magnitude < rhs.magnitude) {
            neg = false;
            magnitude = rhs.magnitude - magnitude;
        }
        else if (magnitude > rhs.magnitude) {
            neg = true;
            magnitude -= rhs.magnitude;
        }
    }
    return *this;
}


BigInt& BigInt::operator*=(const BigInt &rhs) {
    bool negative = (neg != rhs.neg); 
    *this = karatsuba(*this, rhs);
    neg = negative;
    return *this;
}


BigInt BigInt:: operator/=(const BigInt &rhs) {
    if (rhs.magnitude > magnitude) {
        magnitude = 0;
        neg = false;
    }
    else {
        neg = (neg != rhs.neg);
        magnitude /= rhs.magnitude;
    }
    return *this;
}


BigInt& BigInt::operator++() {
    *this += 1;
    return *this;
}


BigInt& BigInt::operator--() {
    *this -= 1;
    return *this;
}


BigInt BigInt::operator++(int) {
    BigInt pre(*this);
    *this += 1;
    return pre;
}


BigInt BigInt::operator--(int) {
    BigInt pre(*this);
    *this -= 1;
    return pre;
}


BigInt operator+(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) += rhs;
}


BigInt operator-(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) -= rhs;
} 


BigInt operator*(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) *= rhs;
}


BigInt operator/(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) /= rhs;
}


bool operator>(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.neg && !rhs.neg) {
        return false;
    }
    else if (!lhs.neg && rhs.neg) {
        return true;
    }
    else if (lhs.neg && rhs.neg) {
        return (lhs.magnitude < rhs.magnitude);
    }
    else {
        return (lhs.magnitude > rhs.magnitude);
    }
}


bool operator<(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.neg && !rhs.neg) {
        return true;
    }
    if (!lhs.neg && rhs.neg) {
        return false;
    }
    if (lhs.neg && rhs.neg) {
        return (lhs.magnitude > rhs.magnitude);
    }
    else {
        return (lhs.magnitude < rhs.magnitude);
    }
}


bool operator==(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.neg != rhs.neg) {
        return false;
    }
    return (lhs.magnitude == rhs.magnitude);
}


bool operator!=(const BigInt &lhs, const BigInt &rhs) {
    return !(lhs == rhs);
}


bool operator>=(const BigInt &lhs, const BigInt &rhs) {
    return (lhs == rhs || lhs > rhs);
}


bool operator<=(const BigInt &lhs, const BigInt &rhs) {
    return (lhs == rhs || lhs < rhs);
}


BigInt& BigInt::set_karatsuba_thres(size_t thres) {
    this->karatsuba_thres = thres;
    return *this;
}


BigInt BigInt::karatsuba(BigInt lhs, BigInt rhs) {
    if (lhs.get_length() < karatsuba_thres || rhs.get_length() < karatsuba_thres) {
        return lhs.magnitude *= rhs.magnitude;
    }
    int m = std::max(lhs.get_length(), rhs.get_length());
    int m2 = std::ceil(m / 2);

    BigInt high1, low1, high2, low2;
    if (lhs.magnitude.get_length() < m2) {
        high1 = 0;
        low1 = lhs;
    }
    else {
        high1 = lhs.magnitude.get_slice(0, lhs.magnitude.get_length() - m2);
        low1 = lhs.magnitude.get_slice(lhs.magnitude.get_length() - m2, lhs.magnitude.get_length());
    }
    if (rhs.magnitude.get_length() < m2) {
        high2 = 0;
        low2 = rhs;
    }
    else {
        high2 = rhs.magnitude.get_slice(0, rhs.magnitude.get_length() - m2);
        low2 = rhs.magnitude.get_slice(rhs.magnitude.get_length() - m2, rhs.magnitude.get_length());
    }

    BigInt z0 = karatsuba(low1, low2);
    BigInt z1 = karatsuba((low1 + high1), (low2 + high2));
    BigInt z2 = karatsuba(high1, high2);

    return ((z2.shift10(m2*2)) + ((z1-z2-z0).shift10(m2)) + z0);
}


BigInt& BigInt::randomize(const size_t &length) {
    neg = std::rand() % 2;
    *this = BigInt(UBigInt().randomize(length), neg);
    return *this;
}

BigInt& BigInt::power(const BigInt &rhs) {
    if (rhs == 0) {
        *this = {1};
    }
    else if (rhs < 0) {
        *this = {0}; 
    }
    else {
        const BigInt base = *this;
        UBigInt count {0};
        while (count < rhs-1) {
            *this *= base;
            count++;
        }
    }
    return *this;
}

BigInt& BigInt::abs() {
    neg = false;
    return *this;
}

BigInt& BigInt::shift10(int m) {
    magnitude.shift10(m);
    return *this;
}

#endif
