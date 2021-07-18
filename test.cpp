#include <cassert>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

#include "bigint.h"

void find_karatsuba_thres(std::pair<int, int> k_test_limits={16,128},
                          int k_test_increment=16,
                          int n_per_increment=5,
                          size_t size_number=1000) {
    /*
    Calculates optimal threshold number of digits to do Karatsuba multiplication on your machine.
    */
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

}


bool test_arithmatic() {
    BigInt ta("123");
    BigInt tb("-123");
    BigInt tc("48");
    BigInt td("-48");
    BigInt te("-309334902454");
    BigInt tf("2342346");
    BigInt tg("4928");
    BigInt th("7423958");
    BigInt ti("4009281734901283749012384721908472198472109837409123809847985685609847982356978364038976501389653019856398739218732109876021945821094582109487632194576231946032194750329147829823195632985");
    BigInt tj("72046120374610239849186598742395873129058732904587320958620319483208989764043890389275");
    BigInt tz("0");

    assert((ta+tz) == ta);
    assert((ta+ta) == BigInt("246"));
    assert((ta+tb) == BigInt("0"));
    assert((ta+tc) == BigInt("171"));
    assert((ta+td) == BigInt("75"));
    assert((ta-ta) == BigInt("0"));
    assert((ta-tb) == BigInt("246"));
    assert((ta-tc) == BigInt("75"));
    assert((ta-td) == BigInt("171"));
    assert((tb+tz) == tb);
    assert((tb+ta) == BigInt("0"));
    assert((tb+tb) == BigInt("-246"));
    assert((tb+tc) == BigInt("-75"));
    assert((tb+td) == BigInt("-171"));
    assert((tb-ta) == BigInt("-246"));
    assert((tb-tb) == BigInt("0"));
    assert((tb-tc) == BigInt("-171"));
    assert((tb-td) == BigInt("-75"));
    assert((tc+ta) == BigInt("171"));
    assert((tc+tb) == BigInt("-75"));
    assert((tc+tc) == BigInt("96"));
    assert((tc+td) == BigInt("0"));
    assert((tc-ta) == BigInt("-75"));
    assert((tc-tb) == BigInt("171"));
    assert((tc-tc) == BigInt("0"));
    assert((tc-td) == BigInt("96"));
    assert((td+ta) == BigInt("75"));
    assert((td+tb) == BigInt("-171"));
    assert((td+tc) == BigInt("0"));
    assert((td+td) == BigInt("-96"));
    assert((ti+tj) == BigInt("4009281734901283749012384721908472198472109837409123809847985685609847982356978364038976501389653019928444859593342349725208544563490455238546365099163552904652514233538137593867086022260"));
    assert((td-ta) == BigInt("-171"));
    assert((td-tb) == BigInt("75"));
    assert((td-tc) == BigInt("-96"));
    assert((td-td) == BigInt("0"));
    assert((ti-tj) == BigInt("4009281734901283749012384721908472198472109837409123809847985685609847982356978364038976501389653019784352618844121870026835347078698708980428899289988910987411875267120158065779305243710"));

    assert((ta*tz) == BigInt("0"));
    assert((ta*tc) == BigInt("5904"));
    assert((ta*td) == BigInt("-5904"));
    assert((ta*td) == BigInt("-5904"));
    assert((tb*td) == BigInt("5904"));
    assert((te*tf) == BigInt("-724569371423517084"));
    assert((tg*th) == BigInt("36585265024"));
    assert((ti*tj) == BigInt("288853194488424069469729017645963098520805528490909790518888657437800905595420860891087371056334964541297711555645041024743502735931106968133713766969809045713583157817868518916076282763336317917556070288954708137868534155993701010900034642641532496448327276296191680235875"));

    assert((ta/tc) == BigInt("2"));
    assert((ta/td) == BigInt("-2"));
    assert((tc/ta) == BigInt("0"));
    assert((tc/tb) == BigInt("0"));
    assert((tz/ta) == BigInt("0"));
    assert((tz/tb) == BigInt("0"));

    std::cout << "[PASS] Arithmatic" << std::endl;
    return true;
}


void test_util() {
    BigInt ta{"123456789"};
    assert(ta.shift10() == BigInt(1234567890));
    assert(ta.shift10(-1) == BigInt(123456789));
    assert(ta.shift10(3) == BigInt("123456789000"));
    assert(ta.shift10(-4) == BigInt(12345678));

    BigInt tb{"1234567"};
    assert(tb.get_length() == 7);
    assert((tb-tb).get_length() == 1);

    BigInt tc{"452349"};
    assert((tc++) == BigInt("452349"));
    assert((tc) == BigInt("452350"));

    BigInt td{"452349"};
    assert((++td) == BigInt("452350"));

    std::cout << "[PASS] Utilities" << std::endl;
}


int main() {
    test_arithmatic();
    test_util();
    find_karatsuba_thres();
    return 0;
}