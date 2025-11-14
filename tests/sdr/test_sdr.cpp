#include "../../sdr/sdr.hpp"
#include <gtest/gtest.h>

using namespace std;

class SdrHwTest{
    public:
    void revealSetupTx(Sdr& sdr){sdr.setupTx();}
    void revealSetupRx(Sdr& sdr){sdr.setupRx();}
    void revealSetRFParams(Sdr& sdr){sdr.setRFParams();}
};
/**
 * @brief tests that the construction of sdr class reads from yaml correctly
 * 
 * Has unit tests for all variables assigned in the loadConfigFromYaml 
 * fucntion in the sdr class to make sure the function pulls from Yaml
 * correctly
 */
TEST(loadConfigFromYaml, LoadsDefault){
    const string kYamlFile = string(CONFIG_DIR) + "/default.yaml";
    Sdr sdr(kYamlFile);
    //DEVICE
    EXPECT_EQ(sdr.getDeviceArgs(), "num_recv_frames=700,num_send_frames=700,recv_frame_size=11000,send_frame_size=11000");
    EXPECT_EQ(sdr.getSubdev(), "A:A");
    EXPECT_EQ(sdr.getClkRef(), "internal");
    EXPECT_EQ(sdr.getClkRate(), 56e6);
    EXPECT_EQ(sdr.getTxChannels(), "0");
    EXPECT_EQ(sdr.getRxChannels(), "0");
    EXPECT_EQ(sdr.getCpuFormat(), "fc32");
    EXPECT_EQ(sdr.getOtwFormat(), "sc12");

    //RF
    EXPECT_EQ(sdr.getRxRate(), 56e6);
    EXPECT_EQ(sdr.getTxRate(), 56e6);
    EXPECT_EQ(sdr.getFreq(), 450e6);
    EXPECT_EQ(sdr.getRxGain(), 10);
    EXPECT_EQ(sdr.getTxGain(), 10);
    EXPECT_EQ(sdr.getBw(), 56e6);
    EXPECT_EQ(sdr.getTxAnt(), "TX/RX");
    EXPECT_EQ(sdr.getRxAnt(), "RX2");
    EXPECT_EQ(sdr.getTransmit(), true);
}

// Software testing for SDR, all run at the very beginning
// Makes sure all basic functions for SDR exist and work correctly

 /**
  * @brief tests setRFParams function when hardware is connected
  * 
  * makes sure no errors are thrown in the setup
  */
TEST(setRFParams, checkParams){
    const string kYamlFile = string(CONFIG_DIR) + "/default.yaml";
    Sdr sdr(kYamlFile);
    SdrHwTest test;

    sdr.createRadio();
    test.revealSetRFParams(sdr);
    EXPECT_NO_THROW(test.revealSetRFParams(sdr));
}