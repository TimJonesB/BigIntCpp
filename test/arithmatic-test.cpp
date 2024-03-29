#include <gtest/gtest.h>
#include "bigint.h"

/**
 * @brief Unit test (gtest) for addition arithmatic
 */
TEST (arithmatic, addition_test) {
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
    EXPECT_EQ((ta+tz), ta);
    EXPECT_EQ((ta+ta), BigInt("246"));
    EXPECT_EQ((ta+tb), BigInt("0"));
    EXPECT_EQ((ta+tc), BigInt("171"));
    EXPECT_EQ((ta+td), BigInt("75"));
    EXPECT_EQ((ta-ta), BigInt("0"));
    EXPECT_EQ((ta-tb), BigInt("246"));
    EXPECT_EQ((ta-tc), BigInt("75"));
    EXPECT_EQ((ta-td), BigInt("171"));
    EXPECT_EQ((tb+tz), tb);
    EXPECT_EQ((tb+ta), BigInt("0"));
    EXPECT_EQ((tb+tb), BigInt("-246"));
    EXPECT_EQ((tb+tc), BigInt("-75"));
    EXPECT_EQ((tb+td), BigInt("-171"));

}
	

/**
 * @brief Unit test (gtest) for subtraction arithmatic
 */
TEST (arithmatic, subtraction_test) {
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
    EXPECT_EQ((tb-ta), BigInt("-246"));
    EXPECT_EQ((tb-tb), BigInt("0"));
    EXPECT_EQ((tb-tc), BigInt("-171"));
    EXPECT_EQ((tb-td), BigInt("-75"));
    EXPECT_EQ((tc+ta), BigInt("171"));
    EXPECT_EQ((tc+tb), BigInt("-75"));
    EXPECT_EQ((tc+tc), BigInt("96"));
    EXPECT_EQ((tc+td), BigInt("0"));
    EXPECT_EQ((tc-ta), BigInt("-75"));
    EXPECT_EQ((tc-tb), BigInt("171"));
    EXPECT_EQ((tc-tc), BigInt("0"));
    EXPECT_EQ((tc-td), BigInt("96"));
}


/**
 * @brief Unit test (gtest) for multplication arithmatic
 */
TEST (arithmatic, multiplication_test) {
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
    EXPECT_EQ((ta*tz), BigInt("0"));
    EXPECT_EQ((ta*tc), BigInt("5904"));
    EXPECT_EQ((ta*td), BigInt("-5904"));
    EXPECT_EQ((ta*td), BigInt("-5904"));
    EXPECT_EQ((tb*td), BigInt("5904"));
    EXPECT_EQ((te*tf), BigInt("-724569371423517084"));
    EXPECT_EQ((tg*th), BigInt("36585265024"));
    EXPECT_EQ((ti*tj), BigInt("288853194488424069469729017645963098520805528490909790518888657437800905595420860891087371056334964541297711555645041024743502735931106968133713766969809045713583157817868518916076282763336317917556070288954708137868534155993701010900034642641532496448327276296191680235875"));
}


/**
 * @brief Unit test (gtest) for division arithmatic
 */
TEST (arithmatic, division_test) {
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
    EXPECT_EQ((ta/tc), BigInt("2"));
    EXPECT_EQ((ta/td), BigInt("-2"));
    EXPECT_EQ((tc/ta), BigInt("0"));
    EXPECT_EQ((tc/tb), BigInt("0"));
    EXPECT_EQ((tz/ta), BigInt("0"));
    EXPECT_EQ((tz/tb), BigInt("0"));
}
