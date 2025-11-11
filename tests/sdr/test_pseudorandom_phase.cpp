#include <gtest/gtest.h>
#include "../../sdr/pseudorandom_phase.hpp"

// Test that the random generator returns a float
TEST(PseudorandomPhase, ReturnValidFloat) {
    EXPECT_NO_THROW(get_next_phase(true));
    EXPECT_NO_THROW(get_next_phase(false));
}

// Test that a vector of the correct size is returned
TEST(GetNPhases, VectorOfFloats) {
    int n = 10;
    auto result = get_next_n_phases(n, true);
    EXPECT_EQ(result.size(), n);
}

// Test that the vector is empty when n is zero
TEST(GetNPhases, TestForZeroN) {
    auto result = get_next_n_phases(0, true);
    EXPECT_TRUE(result.empty());
}