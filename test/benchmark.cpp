#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <boost/multiprecision/gmp.hpp>
#include "bigint.h"


template <typename BIGINT>
void addition_bench() {
    std::srand(std::time(nullptr));
    size_t n_reps = 5;
    auto start = std::chrono::high_resolution_clock::now();
    for (int length = 1; length < 100; length += 10) {
        std::string s1;
        for (int i = 0; i < length; i++) {
            s1.push_back((std::rand() % 10) +'0');
        }
        std::string s2;
        for (int i = 0; i < length; i++) {
            s2.push_back((std::rand() % 10) +'0');
        }
        if (s1[0] == '0') {
            s1[0] = ((std::rand() % 9 + 1) +'0');
        }
        if (s2[0] == '0') {
            s2[0] = ((std::rand() % 9 + 1) +'0');
        }
        BIGINT bigint1{s1};
        BIGINT bigint2{s2};
        for (int i = 0; i < n_reps; i++) {
            bigint1 + bigint2;
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
    std::cout << "Finished in " << us.count() << " us."  << std::endl;
}


template <typename BIGINT>
void subtraction_bench() {
    std::srand(std::time(nullptr));
    size_t n_reps = 5;
    auto start = std::chrono::high_resolution_clock::now();
    for (int length = 1; length < 100; length += 10) {
        std::string s1;
        for (int i = 0; i < length; i++) {
            s1.push_back((std::rand() % 10) +'0');
        }
        std::string s2;
        for (int i = 0; i < length; i++) {
            s2.push_back((std::rand() % 10) +'0');
        }
        if (s1[0] == '0') {
            s1[0] = ((std::rand() % 9 + 1) +'0');
        }
        if (s2[0] == '0') {
            s2[0] = ((std::rand() % 9 + 1) +'0');
        }
        BIGINT bigint1{s1};
        BIGINT bigint2{s2};
        for (int i = 0; i < n_reps; i++) {
            bigint1 + bigint2;
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
    std::cout << "Finished in " << us.count() << " us."  << std::endl;
}


template <typename BIGINT>
void multiplication_bench() {
    std::srand(std::time(nullptr));
    size_t n_reps = 5;
    auto start = std::chrono::high_resolution_clock::now();
    for (int length = 1; length < 100; length += 10) {
        std::string s1;
        for (int i = 0; i < length; i++) {
            s1.push_back((std::rand() % 10) +'0');
        }
        std::string s2;
        for (int i = 0; i < length; i++) {
            s2.push_back((std::rand() % 10) +'0');
        }
        if (s1[0] == '0') {
            s1[0] = ((std::rand() % 9 + 1) +'0');
        }
        if (s2[0] == '0') {
            s2[0] = ((std::rand() % 9 + 1) +'0');
        }
        BIGINT bigint1{s1};
        BIGINT bigint2{s2};
        for (int i = 0; i < n_reps; i++) {
            bigint1 * bigint2;
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
    std::cout << "Finished in " << us.count() << " us."  << std::endl;
}


template <typename BIGINT>
void division_bench() {
    std::srand(std::time(nullptr));
    size_t n_reps = 5;
    auto start = std::chrono::high_resolution_clock::now();
    for (int length = 1; length < 100; length += 10) {
        std::string s1;
        for (int i = 0; i < length; i++) {
            s1.push_back((std::rand() % 10) +'0');
        }
        std::string s2;
        for (int i = 0; i < length; i++) {
            s2.push_back((std::rand() % 10) +'0');
        }
        if (s1[0] == '0') {
            s1[0] = ((std::rand() % 9 + 1) +'0');
        }
        if (s2[0] == '0') {
            s2[0] = ((std::rand() % 9 + 1) +'0');
        }
        BIGINT bigint1{s1};
        BIGINT bigint2{s2};
        for (int i = 0; i < n_reps; i++) {
            bigint1 / bigint2;
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
    std::cout << "Finished in " << us.count() << " us."  << std::endl;
}


TEST (benchmark, benchmark_addition){
    std::cout << "Boost GMP wrapper performance: " << std::endl;
    addition_bench<boost::multiprecision::mpz_int>();
    std::cout << "BigIntCpp wrapper performance: " << std::endl;
    addition_bench<BigInt>();
    EXPECT_EQ(1,1);
}


TEST (benchmark, benchmark_subtraction){
    std::cout << "Boost GMP wrapper performance: " << std::endl;
    subtraction_bench<boost::multiprecision::mpz_int>();
    std::cout << "BigIntCpp wrapper performance: " << std::endl;
    subtraction_bench<BigInt>();
    EXPECT_EQ(1,1);
}


TEST (benchmark, benchmark_multiplication){
    std::cout << "Boost GMP wrapper performance: " << std::endl;
    multiplication_bench<boost::multiprecision::mpz_int>();
    std::cout << "BigIntCpp wrapper performance: " << std::endl;
    multiplication_bench<BigInt>();
    EXPECT_EQ(1,1);
}


TEST (benchmark, benchmark_division){
    std::cout << "Boost GMP wrapper performance: " << std::endl;
    division_bench<boost::multiprecision::mpz_int>();
    std::cout << "BigIntCpp wrapper performance: " << std::endl;
    division_bench<BigInt>();
    EXPECT_EQ(1,1);
}
