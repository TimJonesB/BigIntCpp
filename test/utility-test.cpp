#include <gtest/gtest.h>
#include "bigint.h"

TEST (utility, utility_tests){
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

}
