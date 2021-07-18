#ifndef UBIGINT_H
#define UBIGINT_H

#include <deque>
#include <string>
#include <stack>
#include <type_traits>
#include <unordered_map>


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

UBigInt::UBigInt(std::string s) {
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


std::ostream& operator<<(std::ostream& out, const UBigInt rhs) {
    for (const auto it : rhs.num) {
        std::cout << it;
    }
    return out;
}


bool operator==(const UBigInt &lhs, const UBigInt &rhs) {
    return lhs.num == rhs.num;
}


bool operator<(const UBigInt &lhs, const UBigInt &rhs) {
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


bool operator>(const UBigInt &lhs, const UBigInt &rhs) {
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


bool operator<=(const UBigInt &lhs, const UBigInt &rhs) {
    return (lhs == rhs || lhs < rhs);
}


bool operator>=(const UBigInt &lhs, const UBigInt &rhs) { 
    return (lhs == rhs || lhs > rhs);
}


bool operator!=(const UBigInt &lhs, const UBigInt &rhs) {
    return !(lhs == rhs);
}


UBigInt& UBigInt::operator+=(const UBigInt &rhs) {
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


UBigInt& UBigInt::operator-=(const UBigInt &rhs) {
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


UBigInt& UBigInt::operator*=(const UBigInt &rhs) {
    *this = elementary_mult(*this, rhs);
    return *this;
}


UBigInt& UBigInt::operator/=(const UBigInt &rhs) {
    *this = long_division(rhs);
    return *this;
}


UBigInt& UBigInt::operator++() {
    *this += 1;
    return *this;
}


UBigInt& UBigInt::operator--() {
    *this -= 1;
    return *this;
}


UBigInt UBigInt::operator++(int) {
    UBigInt pre(*this);
    *this += 1;
    return pre;
}


UBigInt UBigInt::operator--(int) {
    UBigInt pre(*this);
    *this -= 1;
    return pre;
}


UBigInt operator+(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) += rhs;
}


UBigInt operator-(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) -= rhs;
} 


UBigInt operator*(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) *= rhs;
}


UBigInt operator/(const UBigInt &lhs, const UBigInt &rhs) {
    return UBigInt(lhs) /= rhs;
}


UBigInt UBigInt::elementary_mult(const UBigInt &lhs, const UBigInt &rhs) {
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


UBigInt UBigInt::divide_primative(const UBigInt &rhs) {
    UBigInt count{0}; 
    auto total = *this;
    while (total >= rhs) {
        total -= rhs;
        count += UBigInt{1};
    }
    return count;
}


UBigInt UBigInt::long_division(const UBigInt &rhs) {
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

UBigInt& UBigInt::randomize(const size_t &length) {
    std::srand(std::time(nullptr));
    std::vector<int> va(length);
    std::generate(va.begin(), va.end(), [](){return std::rand() % 10;});
    *this = UBigInt{va.begin(), va.end()};
    return *this;
}


UBigInt UBigInt::get_slice(size_t start_index, size_t end_index) { 
    while(num[start_index] == 0 && start_index < end_index) {
        start_index++;
    }
    return {num.begin()+start_index, num.begin()+end_index};
}


UBigInt& UBigInt::power(const UBigInt &rhs) {
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


UBigInt& UBigInt::shift10(int m) {
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