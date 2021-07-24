#ifndef UBIGINT_H
#define UBIGINT_H

#include <deque>
#include <string>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <numeric>
#include <algorithm>


/** 
 * @brief Unsigned BigInt class. 
 * Uses std::deque<int> for dynamic storage
 */

class UBigInt {
public:
    UBigInt() = default;
    UBigInt(char rhs): num{rhs-'0'} {}
    UBigInt(std::string s) {
        std::unordered_map<char,int> s2num{{'0',0}, {'1',1}, {'2',2},
                                           {'3',3}, {'4',4}, {'5',5},
                                           {'6',6}, {'7',7}, {'8',8},{'9',9}};
        int i = 0;
        while (s[i] == ' ') {
            i++;
        }
        while (s[i] == '0' || s[i] == '+' || s[i] == '-') {
            i++;
        }
        while (i < s.size()) { 
            if (s2num.find(s[i]) == s2num.end()) {
                throw std::runtime_error("Invalid character in string");
            }
            num.push_back(s2num[s[i]]);
            i++;
        }
        if (num.empty()) {
            num = {0};
        }
    };
    template <class T,
              typename std::enable_if<std::is_integral<T>::value, int>::type* = nullptr>
    UBigInt(T rhs) {
        std::stack<int> st;
        if (rhs == 0) {
            num = {0};
        }
        if (rhs < 0) {
            rhs = -1 * rhs;
        }
        while (rhs > 0) {
            int pop = rhs % 10;
            st.push(pop);
            rhs = rhs/10;
        }
        while (!st.empty()) {
            num.push_back(st.top());
            st.pop();
        }
    }
    template <typename Iter>
    UBigInt(Iter begin, Iter end) : num{begin, end} {}
    UBigInt(const UBigInt &rhs) = default;
    UBigInt(UBigInt &&rhs) = default;
    UBigInt& operator=(const UBigInt &rhs) = default;
    UBigInt& operator=(UBigInt &&rhs) = default;
    ~UBigInt() = default;
    inline UBigInt& operator+=(const UBigInt &rhs);
    inline UBigInt& operator-=(const UBigInt &rhs);
    inline UBigInt& operator*=(const UBigInt &rhs);
    inline UBigInt& operator/=(const UBigInt &rhs);
    inline UBigInt& operator++();
    inline UBigInt& operator--();
    inline UBigInt operator++(int);
    inline UBigInt operator--(int);
    inline friend std::ostream& operator<<(std::ostream &out, const UBigInt rhs);
    inline friend UBigInt operator+(const UBigInt &lhs, const UBigInt &rhs);
    inline friend UBigInt operator-(const UBigInt &lhs, const UBigInt &rhs);
    inline friend UBigInt operator*(const UBigInt &lhs, const UBigInt &rhs);
    inline friend UBigInt operator/(const UBigInt &lhs, const UBigInt &rhs);
    inline friend bool operator<(const UBigInt &lhs, const UBigInt &rhs);
    inline friend bool operator>(const UBigInt &lhs, const UBigInt &rhs);
    inline friend bool operator==(const UBigInt &lhs, const UBigInt &rhs);
    inline friend bool operator<=(const UBigInt &lhs, const UBigInt &rhs);
    inline friend bool operator>=(const UBigInt &lhs, const UBigInt &rhs);
    inline friend bool operator!=(const UBigInt &lhs, const UBigInt &rhs);
    inline UBigInt& power(const UBigInt &rhs);
    inline UBigInt& randomize(const size_t &length);
    inline UBigInt& shift10(int m=1);
    inline UBigInt get_slice(size_t start_index, size_t end_index);
    size_t get_length() {return num.size();}

private:
    std::deque<int> num;
    inline UBigInt elementary_mult(const UBigInt &lhs, const UBigInt &rhs);
    inline UBigInt divide_primative(const UBigInt &rhs);
    inline UBigInt long_division(const UBigInt &rhs);
};



inline std::ostream& operator<<(std::ostream& out, const UBigInt rhs) {
    for (const auto it : rhs.num) {
        std::cout << it;
    }
    return out;
}


/** 
 * @brief Overloaded UBigInt equal to comparison operator 
 * @param lhs UBigInt reference lhs of comparison
 * @param rhs UBigInt reference rhs of comparison
 * @returns True if lhs number is equal to rhs number 
 */
inline bool operator==(const UBigInt &lhs, const UBigInt &rhs) {
    return lhs.num == rhs.num;
}


/** 
 * @brief Overloaded UBigInt less than comparison operator. Compares number of digits then if number(digits) equal, compares the content of digits.
 * @param lhs UBigInt reference lhs of comparison
 * @param rhs UBigInt reference rhs of comparison
 * @returns True if lhs number magnitude is less than rhs number
 */
inline bool operator<(const UBigInt &lhs, const UBigInt &rhs) {
    if (lhs.num.size() < rhs.num.size()) {
        return true;
    }
    else if (lhs.num.size() > rhs.num.size() || lhs == rhs) {
        return false;
    }
    else {
        auto mispair = std::mismatch(lhs.num.begin(), lhs.num.end(), rhs.num.begin());
        return *mispair.first < *mispair.second;
    }
}


/** 
 * @brief Overloaded UBigInt greater than comparison operator. Compares number of digits then if number(digits) equal, compares the content of digits.
 * @param lhs UBigInt reference lhs of comparison
 * @param rhs UBigInt reference rhs of comparison
 * @returns True if lhs number magnitude is greater than rhs number
 */
inline bool operator>(const UBigInt &lhs, const UBigInt &rhs) {
    if (lhs.num.size() > rhs.num.size()) {
        return true;
    }
    else if (lhs.num.size() < rhs.num.size() || lhs == rhs) {
        return false;
    }
    else {
        auto mispair = std::mismatch(lhs.num.begin(), lhs.num.end(), rhs.num.begin());
        return *mispair.first > *mispair.second;
    }
}


/** 
 * @brief Overloaded UBigInt less than or equal to operator.
 * @param lhs UBigInt reference lhs of comparison
 * @param rhs UBigInt reference rhs of comparison
 * @returns True if comparison operator == returns true or operator < returns true
 */
inline bool operator<=(const UBigInt &lhs, const UBigInt &rhs) {
    return (lhs == rhs || lhs < rhs);
}


/** 
 * @brief Overloaded UBigInt greater than or equal to operator.
 * @param lhs UBigInt reference lhs of comparison
 * @param rhs UBigInt reference rhs of comparison
 * @returns True if comparison operator == returns true or operator > returns true
 */
inline bool operator>=(const UBigInt &lhs, const UBigInt &rhs) { 
    return (lhs == rhs || lhs > rhs);
}


/** 
 * @brief Overloaded UBigInt not-equal to comparison operator 
 * @param lhs BigInt reference lhs of comparison
 * @param rhs BigInt reference rhs of comparison
 * @returns True if comparison operator == returns false
 */
inline bool operator!=(const UBigInt &lhs, const UBigInt &rhs) {
    return !(lhs == rhs);
}


/** 
 * @brief Overloaded UBigInt addition assignment operator and core addition algorithm
 * @param rhs UBigInt reference added to *this
 * @returns Reference to modified instance 
 */
inline UBigInt& UBigInt::operator+=(const UBigInt &rhs) {
    int carry = 0;
    for (int i = 0; i < std::max(num.size(), rhs.num.size()); i++) {
        int column_sum = carry;
        if (i < rhs.num.size()) {
            column_sum += rhs.num[rhs.num.size()-i-1];
        }
        if (i < num.size()) {
            column_sum += num[num.size()-i-1];
        }
        else {
            num.push_front(0);
        }
        num[num.size()-i-1] = column_sum > 9 ? (column_sum - 10) : column_sum;
        carry = (column_sum > 9);
    }
    if (carry) {
        num.push_front(carry);
    }
    return *this;
}


/** 
 * @brief Overloaded UBigInt subtraction assignment operator and core subtraction algorithm
 * @param rhs UBigInt reference *this is subtracted by
 * @returns Reference to modified instance 
 */
inline UBigInt& UBigInt::operator-=(const UBigInt &rhs) {
    int borrow = 0;
    if (rhs == *this) {
        *this = 0;
    }
    else if (rhs > *this) {
        throw std::runtime_error("Negative value in UBigInt (unsigned) subtraction.");
    }
    else {
        for (int i = 0; i < num.size(); i++) {
            int column_diff = -1 * borrow;
            if (i < rhs.num.size()) {
                column_diff -= rhs.num[rhs.num.size()-i-1];
            }
            column_diff += num[num.size()-i-1];
            num[num.size()-i-1] = column_diff < 0 ? (column_diff+10) : column_diff;
            borrow = (column_diff < 0);
        }
        if (num.front() == 0) {
            num.pop_front();
        }
    }
    return *this;
}


/** 
 * @brief Overloaded UBigInt multplication assignment operator
 * @param rhs UBigInt reference multplied by *this
 * @returns Reference to modified instance 
 */
inline UBigInt& UBigInt::operator*=(const UBigInt &rhs) {
    *this = elementary_mult(*this, rhs);
    return *this;
}


/** 
 * @brief Overloaded UBigInt division assignment operator
 * @param rhs UBigInt reference *this is divided by
 * @returns Reference to modified instance 
 */
inline UBigInt& UBigInt::operator/=(const UBigInt &rhs) {
    *this = long_division(rhs);
    return *this;
}


/** 
 * @brief Overloaded BigInt postfix increment operator 
 * @returns Reference to modified instance 
 */
inline UBigInt& UBigInt::operator++() {
    *this += 1;
    return *this;
}


/** 
 * @brief Overloaded BigInt postfix decrement operator 
 * @returns Reference to modified instance 
 */
inline UBigInt& UBigInt::operator--() {
    *this -= 1;
    return *this;
}


/** 
 * @brief Overloaded BigInt prefix increment operator 
 * @param _ Placeholder integer argument
 * @returns Copy of new instance 
 */
inline UBigInt UBigInt::operator++(int) {
    UBigInt pre(*this);
    *this += 1;
    return pre;
}


/** 
 * @brief Overloaded BigInt prefix decrement operator 
 * @param _ Placeholder integer argument
 * @returns Copy of new instance 
 */
inline UBigInt UBigInt::operator--(int) {
    UBigInt pre(*this);
    *this -= 1;
    return pre;
}


/** 
 * @brief Overloaded UBigInt binary addition operator 
 * @param lhs UBigInt reference lhs component of sum
 * @param rhs UBigInt reference rhs component of sum
 * @returns Copy of new instance 
 */
inline UBigInt operator+(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) += rhs;
}


/** 
 * @brief Overloaded UBigInt binary subtraction operator 
 * @param lhs UBigInt reference lhs component of difference
 * @param rhs UBigInt reference rhs component of difference
 * @returns Copy of new instance 
 */
inline UBigInt operator-(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) -= rhs;
} 


/** 
 * @brief Overloaded UBigInt binary multiplication operator 
 * @param lhs UBigInt reference lhs component of product
 * @param rhs UBigInt reference rhs component of product
 * @returns Copy of new instance 
 */
inline UBigInt operator*(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) *= rhs;
}


/** 
 * @brief Overloaded UBigInt binary division operator 
 * @param lhs UBigInt reference lhs (numerator) component of product
 * @param rhs UBigInt reference rhs (denominator) component of product
 * @returns Copy of new instance 
 */
inline UBigInt operator/(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) /= rhs;
}


inline UBigInt UBigInt::elementary_mult(const UBigInt &lhs, const UBigInt &rhs) {
    if (lhs == 0 || rhs == 0) {
        return 0;
    }
    const std::deque<int> &top = num.size() >= rhs.num.size() ? num : rhs.num;
    const std::deque<int> &bottom = num.size() < rhs.num.size() ? num : rhs.num;
    std::deque<int> product;
    int carry_mult;
    int carry_add;
    int offset;
    for (int j = 0; j < bottom.size(); j++) {
        carry_mult = 0;
        carry_add = 0;
        for(int i = 0; i < top.size(); i++) {
            offset = i + j;
            int prod = (top[top.size()-i-1] * bottom[bottom.size()-j-1]) + carry_mult;
            int num_mult = prod % 10;
            carry_mult = prod / 10;
            if(product.size() < offset+1) {
                product.push_front(0);
            }
            int sum = product[product.size()-offset-1] + num_mult + carry_add;
            int num_add = (sum > 9) ? (sum - 10) : sum; 
            carry_add = (sum > 9);
            product[product.size()-offset-1]  = num_add;
        }
        if (carry_mult || carry_add) {
            product.push_front(carry_mult + carry_add);
        }
    }
    return UBigInt(product.begin(), product.end());
}


inline UBigInt UBigInt::divide_primative(const UBigInt &rhs) {
    UBigInt count{0}; 
    auto total = *this;
    while (total >= rhs) {
        total -= rhs;
        count += UBigInt{1};
    }
    return count;
}


inline UBigInt UBigInt::long_division(const UBigInt &rhs) {
    UBigInt temp{0};
    UBigInt sol;
    if (rhs > *this) {
        return UBigInt{0};
    }
    else if (rhs == *this) {
        return UBigInt{1};
    }
    else {
        for (int i = 0; i < num.size();) {
            temp = temp * 10 + num[i++];
            UBigInt quo = temp.divide_primative(rhs);
            for (const auto it : quo.num) {
                if (!(sol.num.empty() && quo == 0)) {
                    sol.num.push_back(it);
                }
            }
            if (quo*rhs <= temp) {
                temp = temp - (quo * rhs);
            }
        }
        if (sol.num.empty()) {
            return UBigInt{0};
        }
        else {
            return sol;
        }
    }
}

inline UBigInt& UBigInt::randomize(const size_t &length) {
    std::srand(std::time(nullptr));
    std::vector<int> va(length);
    std::generate(va.begin(), va.end(), [](){return std::rand() % 10;});
    *this = UBigInt{va.begin(), va.end()};
    return *this;
}


inline UBigInt UBigInt::get_slice(size_t start_index, size_t end_index) { 
    while(num[start_index] == 0 && start_index < end_index) {
        start_index++;
    }
    return {num.begin()+start_index, num.begin()+end_index};
}


inline UBigInt& UBigInt::power(const UBigInt &rhs) {
    if (rhs == 0) {
        *this = {1};
    }
    else {
        const UBigInt base = *this;
        UBigInt count {0};
        while (count < rhs-1) {
            *this *= base;
            count++;
        }
    }
    return *this;
}


inline UBigInt& UBigInt::shift10(int m) {
    if(m > 0) {
        num.resize(num.size() + m);
    }
    if(m < 0) {
        for (int i = 0; i < std::abs(m); i++) {
            if (num.empty()) {
                num = {0};
                return *this;
            }
            num.erase(num.end());
        }
    }
    return *this;
}

#endif
