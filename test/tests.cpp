#include <gtest/gtest.h>

/**
 * @brief  Entry point for module which runs all available gtests on BigIntCpp header only library.
 */
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
