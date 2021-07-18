#include <cassert>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

#include "bigint.h"

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
