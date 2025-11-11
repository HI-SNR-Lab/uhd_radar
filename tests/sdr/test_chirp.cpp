#include "../../sdr/chirp.hpp"
#include <gtest/gtest.h>

using namespace std;

/**
 * @brief tests that the construction of chirp class reads from yaml correctly
 * 
 * Has unit tests for all variables assigned in the assignVarFromUYam; 
 * fucntion in the chirp class to make sure the function pulls from Yaml
 * correctly
 */
TEST(assignVarFromYaml, loadsDefault){
    const string kConfigFile = string(CONFIG_DIR) + "/default.yaml";
    Chirp chirp(kConfigFile);

    EXPECT_EQ(chirp.getTimeOffset(), 1);
    EXPECT_EQ(chirp.getTxDuration(), 20e-6);
    EXPECT_EQ(chirp.getRxDuration(), 20e-6);
    EXPECT_EQ(chirp.getTrOnLead(), 0e-6);
    EXPECT_EQ(chirp.getTrOffTrail(), 0e-6);
    EXPECT_EQ(chirp.getPulseRepInt(), 200e-6);
    EXPECT_EQ(chirp.getTxLead(), 0e-6);
    EXPECT_EQ(chirp.getNumPulses(), 10000);
    EXPECT_EQ(chirp.getNumPresums(), 1);
    EXPECT_EQ(chirp.getPhaseDither(), true);
}

/**
 * @brief tests Chirp::setTimeOffset(double value) for valid inputs
 *
 * Time offset [s] may be any non-negative double
 */
TEST(setTimeOffset, validValues){
    const string kConfigFile = string(CONFIG_DIR) + "/default.yaml";
    Chirp chirp(kConfigFile);

    chirp.setTimeOffset(0);
    EXPECT_EQ(chirp.getTimeOffset(), 0);
    chirp.setTimeOffset(2);
    EXPECT_EQ(chirp.getTimeOffset(), 2);
    chirp.setTimeOffset(1.1);
    EXPECT_EQ(chirp.getTimeOffset(), 1.1);
}

/**
 * @brief tests Chirp::setTimeOffset(double value) for invalid inputs
 *
 * Time offset [s] may not be negative
 */
TEST(setTimeOffset, invalidValues){
    const string kConfigFile = string(CONFIG_DIR) + "/default.yaml";
    Chirp chirp(kConfigFile);

    EXPECT_THROW(chirp.setTimeOffset(-2), invalid_argument);
}

/**
 * @brief tests Chirp::setMaxChirpsPerFile(int value) for valid inputs
 *
 * Max chirps per file may be any positive integer, or -1 to disable file-splitting.
 */
TEST(setMaxChirpsPerFile, validValues) {
    const string kConfigFile = string(CONFIG_DIR) + "/default.yaml";
    Chirp chirp(kConfigFile);

    chirp.setMaxChirpsPerFile(-1);
    EXPECT_EQ(chirp.getMaxChirpsPerFile(), -1);
    chirp.setMaxChirpsPerFile(1);
    EXPECT_EQ(chirp.getMaxChirpsPerFile(), 1);
}

/**
 * @brief tests Chirp::setMaxChirpsPerFile(int value) for invalid inputs
 *
 * Max chirps per file may not be 0 or any negative integer except -1
 */
TEST(setMaxChirpsPerFile, invalidValues) {
    const string kConfigFile = string(CONFIG_DIR) + "/default.yaml";
    Chirp chirp(kConfigFile);

    EXPECT_THROW(chirp.setMaxChirpsPerFile(0), invalid_argument);
    EXPECT_THROW(chirp.setMaxChirpsPerFile(-2), invalid_argument);
}
