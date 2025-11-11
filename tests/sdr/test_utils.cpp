#include <gtest/gtest.h>
#include "../../sdr/utils.hpp"

// Test that the base filename is returned when only one filename is to be generated
TEST(GenerateOutFilename, OneFilename) {
    EXPECT_EQ(generate_out_filename("usrp_samples.dat", 1, 0), "usrp_samples.dat");
}

// Test zero padding of the filename when two names are being generated
TEST(GenerateOutFilename, MultipleFilenames) {
    EXPECT_EQ(generate_out_filename("usrp_samples.dat", 2, 0), "usrp_samples.00.dat");
    EXPECT_EQ(generate_out_filename("usrp_samples.dat", 2, 1), "usrp_samples.01.dat");
}

// TODO: test edge cases of generate_out_filename() such as many (>99) files generated, multiple .'s in base_fn, etc.
