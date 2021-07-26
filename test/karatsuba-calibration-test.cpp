#include <gtest/gtest.h>
#include "bigint.h"
#include <chrono>
#include <algorithm>
#include <numeric>

/**
 * @brief Unit test (gtest) for division arithmatic. Always returns "PASS" but informs user if they should change the threshold from default.
 * The karatsuba threshold is the minimum length (in digits) that use karatsuba recursive multiplication algorithm vs elementary multiplication (base-case) for recursion.
 */
TEST (karatsuba, karatsuba_calibration) {
	std::pair<int, int> k_test_limits= {16,128};
    int k_test_increment = 16;
    int n_per_increment = 5;
    size_t size_number=1000 ;
    BigInt a = BigInt().randomize(size_number);
    BigInt b = BigInt().randomize(size_number);
    std::vector<std::pair<int, double>> kthres_vs_time;
    std::cout << "Running Karatsuba Multiplication size threshold calibration..." << std::endl;
    std::cout << "Current Karatsuba Multiplication default threshold : " << BigInt().get_karatsuba_thres() << std::endl;
    for (int kthres = k_test_limits.first; kthres <= k_test_limits.second; kthres += k_test_increment) {
        std::cout << "Testing threshold = " << kthres << std::endl;
        a.set_karatsuba_thres(kthres);
        b.set_karatsuba_thres(kthres);
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

    std::cout << "Optimal karatsuba digits threshold = " << optim_digits->first << std::endl;
    if (optim_digits->first != BigInt().get_karatsuba_thres()) {
        std::cout << "Warning: optimal karatsuba multiplication threshold different than current default threshold. It is recommended to change to optimal threshold." << std::endl;
    }
    else {
        std::cout << "Default karatsuba multiplication threshold is optimal" << std::endl;
    }
	EXPECT_EQ(1,1);
}
