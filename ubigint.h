#ifndef UBIGINT_H
#define UBIGINT_H

#include <deque>
#include <string>
#include <stack>
#include <type_traits>

class UBigInt {
public:
    UBigInt() = default;
    UBigInt(char rhs): num{rhs-'0'} {}
    UBigInt(std::string rhs);
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
    UBigInt& operator+=(const UBigInt &rhs);
    UBigInt& operator-=(const UBigInt &rhs);
    UBigInt& operator*=(const UBigInt &rhs);
    UBigInt& operator/=(const UBigInt &rhs);
    UBigInt& operator++();
    UBigInt& operator--();
    UBigInt operator++(int);
    UBigInt operator--(int);
    friend std::ostream& operator<<(std::ostream &out, const UBigInt rhs);
    friend UBigInt operator+(const UBigInt &lhs, const UBigInt &rhs);
    friend UBigInt operator-(const UBigInt &lhs, const UBigInt &rhs);
    friend UBigInt operator*(const UBigInt &lhs, const UBigInt &rhs);
    friend UBigInt operator/(const UBigInt &lhs, const UBigInt &rhs);
    friend bool operator<(const UBigInt &lhs, const UBigInt &rhs);
    friend bool operator>(const UBigInt &lhs, const UBigInt &rhs);
    friend bool operator==(const UBigInt &lhs, const UBigInt &rhs);
    friend bool operator<=(const UBigInt &lhs, const UBigInt &rhs);
    friend bool operator>=(const UBigInt &lhs, const UBigInt &rhs);
    friend bool operator!=(const UBigInt &lhs, const UBigInt &rhs);
    UBigInt& power(const UBigInt &rhs);
    UBigInt& randomize(const size_t &length);
    UBigInt& shift10(int m=1);
    size_t get_length() {return num.size();}
    UBigInt get_slice(size_t start_index, size_t end_index);

private:
    std::deque<int> num;
    UBigInt elementary_mult(const UBigInt &lhs, const UBigInt &rhs);
    UBigInt divide_primative(const UBigInt &rhs);
    UBigInt long_division(const UBigInt &rhs);
};

#endif