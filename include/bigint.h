#ifndef BIGINT_H
#define BIGINT_H

#include <type_traits>
#include <iostream>
#include <cmath>

#include "ubigint.h"

/** 
 * @brief Signed arbitrarily "big" precision integer class.
 * Uses std::deque<int> for dynamic storage. Stores magnitude by composition in unsigned UBigInt member.
 */

class BigInt {
public:
    BigInt() = default;
    BigInt(char rhs, bool negative=0): magnitude{rhs}, neg{negative} {}
    BigInt(std::string s) {
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
    inline  ~BigInt() = default;
    inline  BigInt& operator=(const BigInt &rhs) = default;
    inline  BigInt& operator=(BigInt &&rhs) = default;
    inline  BigInt& operator+=(const BigInt &rhs);
    inline  BigInt& operator-=(const BigInt &rhs);
    inline  BigInt& operator*=(const BigInt &rhs);
    inline  BigInt operator/=(const BigInt &rhs);
    inline  BigInt& operator++();
    inline  BigInt& operator--();
    inline  BigInt operator++(int);
    inline  BigInt operator--(int);
    inline  friend BigInt operator+(const BigInt &lhs, const BigInt &rhs);
    inline  friend BigInt operator-(const BigInt &lhs, const BigInt &rhs);
    inline  friend BigInt operator*(const BigInt &lhs, const BigInt &rhs);
    inline  friend BigInt operator/(const BigInt &lhs, const BigInt &rhs);
    inline  friend bool operator>(const BigInt &lhs, const BigInt &rhs);
    inline  friend bool operator<(const BigInt &lhs, const BigInt &rhs);
    inline  friend bool operator==(const BigInt &lhs, const BigInt &rhs);
    inline  friend bool operator!=(const BigInt &lhs, const BigInt &rhs);
    inline  friend bool operator>=(const BigInt &lhs, const BigInt &rhs);
    inline  friend bool operator<=(const BigInt &lhs, const BigInt &rhs);
    inline  friend std::ostream& operator<<(std::ostream &out, const BigInt &rhs);
    inline  BigInt& randomize(const size_t &length);
    inline  BigInt& power(const BigInt &rhs);
    inline  BigInt& shift10(int m=1);
    inline  BigInt& abs();
    inline  BigInt& set_karatsuba_thres(size_t thres); 
    size_t get_length() {return magnitude.get_length();}
    size_t get_karatsuba_thres() {return this->karatsuba_thres;}
private:
    UBigInt magnitude;
    bool neg = false;
    size_t karatsuba_thres = 48;
    inline BigInt karatsuba(BigInt lhs, BigInt rhs);
};



/** 
 * @brief Overloaded BigInt insertion operator prints sign and value of BigInt
 * @param out Output stream reference
 * @param rhs Subject BigInt refence to stream
 * @returns Reference to output stream
 */
inline std::ostream& operator<<(std::ostream &out, const BigInt &rhs) {
    if (rhs.neg) {
        std::cout << '-';
    }
    std::cout << rhs.magnitude; 
    return out;
}


/** 
 * @brief Overloaded BigInt addition assignment operator 
 * @param rhs BigInt reference added to *this
 * @returns Reference to modified instance 
 */
inline BigInt& BigInt::operator+=(const BigInt &rhs) {
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


/** 
 * @brief Overloaded BigInt subtraction assignment operator 
 * @param rhs BigInt reference subtracted from *this
 * @returns Reference to modified instance 
 */
inline BigInt& BigInt::operator-=(const BigInt &rhs) {
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


/** 
 * @brief Overloaded BigInt multiplication assignment operator 
 * @param rhs BigInt reference multiplied by *this
 * @returns Reference to modified instance 
 */
inline BigInt& BigInt::operator*=(const BigInt &rhs) {
    bool negative = (neg != rhs.neg); 
    *this = karatsuba(*this, rhs);
    neg = negative;
    return *this;
}


/** 
 * @brief Overloaded BigInt division assignment operator 
 * @param rhs BigInt reference *this is divided by
 * @returns Reference to modified instance 
 */
inline BigInt BigInt:: operator/=(const BigInt &rhs) {
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


/** 
 * @brief Overloaded BigInt postfix increment operator 
 * @returns Reference to modified instance 
 */
inline BigInt& BigInt::operator++() {
    *this += 1;
    return *this;
}


/** 
 * @brief Overloaded BigInt postfix decrement operator 
 * @returns Reference to modified instance 
 */
inline BigInt& BigInt::operator--() {
    *this -= 1;
    return *this;
}

/** 
 * @brief Overloaded BigInt prefix increment operator 
 * @param _ Placeholder integer argument
 * @returns Copy of new instance 
 */
inline BigInt BigInt::operator++(int) {
    BigInt pre(*this);
    *this += 1;
    return pre;
}


/** 
 * @brief Overloaded BigInt prefix decrement operator 
 * @param _ Placeholder integer argument
 * @returns Copy of new instance 
 */
inline BigInt BigInt::operator--(int) {
    BigInt pre(*this);
    *this -= 1;
    return pre;
}


/** 
 * @brief Overloaded BigInt binary addition operator 
 * @param lhs BigInt reference lhs component of sum
 * @param rhs BigInt reference rhs component of sum
 * @returns Copy of new instance 
 */
inline BigInt operator+(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) += rhs;
}


/** 
 * @brief Overloaded BigInt binary subtraction operator 
 * @param lhs BigInt reference lhs component of difference
 * @param rhs BigInt reference rhs component of difference
 * @returns Copy of new instance 
 */
inline BigInt operator-(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) -= rhs;
} 


/** 
 * @brief Overloaded BigInt binary multiplication operator 
 * @param lhs BigInt reference lhs component of product
 * @param rhs BigInt reference rhs component of product
 * @returns Copy of new instance 
 */
inline BigInt operator*(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) *= rhs;
}


/** 
 * @brief Overloaded BigInt binary division operator 
 * @param lhs BigInt reference lhs (numerator) component of product
 * @param rhs BigInt reference rhs (denominator) component of product
 * @returns Copy of new instance 
 */
inline BigInt operator/(const BigInt &lhs, const BigInt &rhs) {
    return BigInt(lhs) /= rhs;
}


/** 
 * @brief Overloaded BigInt greater than comparison operator 
 * @param lhs BigInt reference lhs of comparison
 * @param rhs BigInt reference rhs of comparison
 * @returns True if lhs > rhs 
 */

inline bool operator>(const BigInt &lhs, const BigInt &rhs) {
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


/** 
 * @brief Overloaded BigInt less than comparison operator 
 * @param lhs BigInt reference lhs of comparison
 * @param rhs BigInt reference rhs of comparison
 * @returns True if lhs < rhs 
 */
inline bool operator<(const BigInt &lhs, const BigInt &rhs) {
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


/** 
 * @brief Overloaded BigInt equal to comparison operator 
 * @param lhs BigInt reference lhs of comparison
 * @param rhs BigInt reference rhs of comparison
 * @returns True if lhs magnitude and sign == rhs magnitude and sign
 */
inline bool operator==(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.neg != rhs.neg) {
        return false;
    }
    return (lhs.magnitude == rhs.magnitude);
}


/** 
 * @brief Overloaded BigInt not-equal to comparison operator 
 * @param lhs BigInt reference lhs of comparison
 * @param rhs BigInt reference rhs of comparison
 * @returns True if comparison operator == returns false
 */
inline bool operator!=(const BigInt &lhs, const BigInt &rhs) {
    return !(lhs == rhs);
}


/** 
 * @brief Overloaded BigInt greater than or equal to comparison operator 
 * @param lhs BigInt reference lhs of comparison
 * @param rhs BigInt reference rhs of comparison
 * @returns True if comparison operator == returns true or operator > returns true
 */
inline bool operator>=(const BigInt &lhs, const BigInt &rhs) {
    return (lhs == rhs || lhs > rhs);
}


/** 
 * @brief Overloaded BigInt less than or equal to comparison operator 
 * @param lhs BigInt reference lhs of comparison
 * @param rhs BigInt reference rhs of comparison
 * @returns True if comparison operator == returns true or operator < returns true
 */
inline bool operator<=(const BigInt &lhs, const BigInt &rhs) {
    return (lhs == rhs || lhs < rhs);
}


/** 
 * @brief Sets karatsuba_thres which is minimum size(magnitude) that uses karatsuba vs. elementary multiplication
 * @param thres Threshold size
 * @returns Reference to adjusted instance
 */
inline BigInt& BigInt::set_karatsuba_thres(size_t thres) {
    this->karatsuba_thres = thres;
    return *this;
}


/** 
 * @brief Karatsuba recursive multiplication algorithm which recurses at size(magnitude) > karatsuba_thres. Base condition calls elementary multiplication
 * @param lhs Left hand portion of multiplication algorithm
 * @param rhs Right hand portion of multiplication algorithm
 * @returns Copy of BigInt product
 */
inline BigInt BigInt::karatsuba(BigInt lhs, BigInt rhs) {
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


/** 
 * @brief  Randomizes BigInt instance sign and magnitude to specified number of digits
 * @param length Specified number of digits
 * @returns Reference to modified BigInt
 */
inline BigInt& BigInt::randomize(const size_t &length) {
    neg = std::rand() % 2;
    *this = BigInt(UBigInt().randomize(length), neg);
    return *this;
}


/** 
 * @brief  BigInt's exponent utility method
 * @param rhs Exponent to raise base *this by
 * @returns Reference to modified BigInt
 */
inline BigInt& BigInt::power(const BigInt &rhs) {
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


/** 
 * @brief  BigInt's absolute value utility method
 * @returns Reference to modified BigInt
 */
inline BigInt& BigInt::abs() {
    neg = false;
    return *this;
}


/** 
 * @brief  BigInt's base-10 shift utility method, calls member magnitudes (UBigInt) shift method
 * @param m Number of places to shift forward or back
 * @returns Reference to modified BigInt
 */
inline BigInt& BigInt::shift10(int m) {
    magnitude.shift10(m);
    return *this;
}


#endif
