#include <gtest/gtest.h>
#include "bigint.h"

TEST (utility, utility_tests){
    BigInt ta{"123456789"};
    EXPECT_EQ(ta.shift10(), BigInt(1234567890));
    EXPECT_EQ(ta.shift10(-1), BigInt(123456789));
    EXPECT_EQ(ta.shift10(3), BigInt("123456789000"));
    EXPECT_EQ(ta.shift10(-4), BigInt(12345678));

    BigInt tb{"1234567"};
    EXPECT_EQ(tb.get_length(), 7);
    EXPECT_EQ((tb-tb).get_length(), 1);

    BigInt tc{"452349"};
    EXPECT_EQ(tc++, BigInt("452349"));
    EXPECT_EQ(tc, BigInt("452350"));

    BigInt td{"452349"};
    EXPECT_EQ((++td), BigInt("452350"));
}
