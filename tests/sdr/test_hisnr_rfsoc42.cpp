#include "../../sdr/hisnr_rfsoc42.hpp"
#include <gtest/gtest.h>

/** Needs to be updated */

using namespace std;

class SdrHwTest{
    public:
    void revealCheckAndSetTime(HiSnrRFSoC42& sdr){sdr.checkAndSetTime();}
    void revealSetRFParams(HiSnrRFSoC42& sdr){sdr.setRFParams();}
    void revealSetupTx(HiSnrRFSoC42& sdr){sdr.setupTx();}
    void revealSetupRx(HiSnrRFSoC42& sdr){sdr.setupRx();}
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
    HiSnrRFSoC42 sdr(kYamlFile);
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

/**
 * @brief tests GPIO mask getter functions
 *
 * these 4 getter methods are not tested with the other getter
 * methods in Loads Default because they are not directly read from YAML
 */
TEST(GetterMethods, UntestedInLoadsDefault){
    const string kYamlFile = string(CONFIG_DIR) + "/default.yaml";
    HiSnrRFSoC42 sdr(kYamlFile);
}


//hardware testing for usrp, all run at the very beginning

/** 
 * @brief tests checkAndSetTime function when hardware and gps are connected
 * 
 * this makes sure that the expected time and the actual time are the same
 */
TEST(checkAndSetTime, checkParams){
    const string kYamlFile = string(CONFIG_DIR) + "/default.yaml";
    HiSnrRFSoC42 sdr(kYamlFile);
    SdrHwTest test;

    sdr.createRadio();
    if(sdr.getClkRef() == "gpsdo"){
        test.revealCheckAndSetTime(sdr);
        // EXPECT_EQ(expected_time, actual_time); //should pass if USRP time is synchronized to GPS time
    //does this need to exist
}}


 /**
  * @brief tests setRFParams function when hardware is connected
  * 
  * makes sure no errors are thrown in the setup
  */
TEST(setRFParams, checkParams){
    const string kYamlFile = string(CONFIG_DIR) + "/default.yaml";
    HiSnrRFSoC42 sdr(kYamlFile);
    SdrHwTest test;

    sdr.createRadio();

    // sdr.getUsrp()->set_time_next_pps(time_spec_t(0.0));
    // this_thread::sleep_for((chrono::milliseconds(1000)));
    // if (sdr.getTransmit()) {
    //     sdr.getUsrp()->set_tx_subdev_spec(sdr.getSubdev());
    //  }
    // sdr.getUsrp()->set_rx_subdev_spec(sdr.getSubdev());
    // sdr.getUsrp()->set_master_clock_rate(sdr.getClkRate());

    test.revealSetRFParams(sdr);
    // EXPECT_NO_THROW(test.revealSetRFParams(sdr));
}
