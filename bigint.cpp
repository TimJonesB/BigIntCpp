#include "bigint.h"
#include "ubigint.h"

#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <unordered_map>
#include <cstdlib>
#include <stdexcept>
#include <climits>
#include <deque>
#include <cmath>
#include <cassert>
#include <chrono>
#include <numeric>
#include <algorithm>


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


BigInt& BigInt::shift10(int m) {
    magnitude.shift10(m);
    return *this;
}



void find_karatsuba_thres(std::pair<int, int> k_test_limits,
                          int k_test_increment,
                          int n_per_increment,
                          size_t size_number) {
    /*
    Calculates optimal threshold number of digits to do Karatsuba multiplication on your machine.
    Make BigInt.karatsuba_thres public prior to use.
    */
    BigInt a = BigInt().randomize(size_number);
    BigInt b = BigInt().randomize(size_number);
    std::vector<std::pair<int, double>> kthres_vs_time;
    for (int kthres = k_test_limits.first; kthres < k_test_limits.second; kthres += k_test_increment) {
        a.karatsuba_thres = kthres;
        b.karatsuba_thres = kthres;
        std::vector<double> itimes;
        for (int i = 0; i < n_per_increment; i++){
            std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
            auto sol = a*b;
            std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
            itimes.push_back(time_span.count());
        }
        double sum = std::accumulate(std::begin(itimes), std::end(itimes), 0.0);
        double mean =  sum / itimes.size();
        kthres_vs_time.push_back({kthres, mean});
    }
    auto optim_digits = std::min_element(kthres_vs_time.begin(), kthres_vs_time.end(),
                                  [](const auto &lhs, const auto &rhs) {return lhs.second < rhs.second;});

    std::cout << "Optimal Karatsuba Digits Threshold = " << optim_digits->first << std::endl;
}


bool test_arithmatic() {
    BigInt a = BigInt("123");
    BigInt b = BigInt("-123");
    BigInt c = BigInt("48");
    BigInt d = BigInt("-48");
    BigInt e = BigInt("-309334902454");
    BigInt f = BigInt("2342346");
    BigInt g = BigInt("4928");
    BigInt h = BigInt("7423958");
    BigInt i = BigInt("4009281734901283749012384721908472198472109837409123809847985685609847982356978364038976501389653019856398739218732109876021945821094582109487632194576231946032194750329147829823195632985");
    BigInt j = BigInt("72046120374610239849186598742395873129058732904587320958620319483208989764043890389275");
    BigInt z = BigInt("0");

    assert((a+z) == a);
    assert((a+a) == BigInt("246"));
    assert((a+b) == BigInt("0"));
    assert((a+c) == BigInt("171"));
    assert((a+d) == BigInt("75"));
    assert((a-a) == BigInt("0"));
    assert((a-b) == BigInt("246"));
    assert((a-c) == BigInt("75"));
    assert((a-d) == BigInt("171"));
    assert((b+z) == b);
    assert((b+a) == BigInt("0"));
    assert((b+b) == BigInt("-246"));
    assert((b+c) == BigInt("-75"));
    assert((b+d) == BigInt("-171"));
    assert((b-a) == BigInt("-246"));
    assert((b-b) == BigInt("0"));
    assert((b-c) == BigInt("-171"));
    assert((b-d) == BigInt("-75"));
    assert((c+a) == BigInt("171"));
    assert((c+b) == BigInt("-75"));
    assert((c+c) == BigInt("96"));
    assert((c+d) == BigInt("0"));
    assert((c-a) == BigInt("-75"));
    assert((c-b) == BigInt("171"));
    assert((c-c) == BigInt("0"));
    assert((c-d) == BigInt("96"));
    assert((d+a) == BigInt("75"));
    assert((d+b) == BigInt("-171"));
    assert((d+c) == BigInt("0"));
    assert((d+d) == BigInt("-96"));
    assert((i+j) == BigInt("4009281734901283749012384721908472198472109837409123809847985685609847982356978364038976501389653019928444859593342349725208544563490455238546365099163552904652514233538137593867086022260"));
    assert((d-a) == BigInt("-171"));
    assert((d-b) == BigInt("75"));
    assert((d-c) == BigInt("-96"));
    assert((d-d) == BigInt("0"));
    assert((i-j) == BigInt("4009281734901283749012384721908472198472109837409123809847985685609847982356978364038976501389653019784352618844121870026835347078698708980428899289988910987411875267120158065779305243710"));

    assert((a*z) == BigInt("0"));
    assert((a*c) == BigInt("5904"));
    assert((a*d) == BigInt("-5904"));
    assert((a*d) == BigInt("-5904"));
    assert((b*d) == BigInt("5904"));
    assert((e*f) == BigInt("-724569371423517084"));
    assert((g*h) == BigInt("36585265024"));
    assert((i*j) == BigInt("288853194488424069469729017645963098520805528490909790518888657437800905595420860891087371056334964541297711555645041024743502735931106968133713766969809045713583157817868518916076282763336317917556070288954708137868534155993701010900034642641532496448327276296191680235875"));

    assert((a/c) == BigInt("2"));
    assert((a/d) == BigInt("-2"));
    assert((c/a) == BigInt("0"));
    assert((c/b) == BigInt("0"));
    assert((z/a) == BigInt("0"));
    assert((z/b) == BigInt("0"));

    std::cout << "[PASS] Arithmatic" << std::endl;
}


void test_util() {
    BigInt a{"123456789"};
    assert(a.shift10() == BigInt(1234567890));
    assert(a.shift10(-1) == BigInt(123456789));
    assert(a.shift10(3) == BigInt("123456789000"));
    assert(a.shift10(-4) == BigInt(12345678));

    BigInt b{"1234567"};
    assert(b.get_length() == 7);
    assert((b-b).get_length() == 1);

    BigInt c{"452349"};
    assert((c++) == BigInt("452349"));
    assert((c) == BigInt("452350"));

    BigInt d{"452349"};
    assert((++d) == BigInt("452350"));

    std::cout << "[PASS] Utilities" << std::endl;
}
