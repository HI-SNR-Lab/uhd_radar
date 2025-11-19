#ifndef HISNR_USRP_HPP
#define HISNR_USRP_HPP

#include "yaml-cpp/yaml.h"
#include "usrp_rf_settings.hpp"
#include "common.hpp"
#include "sdr.hpp"

class HiSnrUsrp : public Sdr {

  public:
    HiSnrUsrp() = delete;
    HiSnrUsrp(const string& kYamlFile);

    void createRadio() override;
    void setupRadio() override;

    // GPIO
    int getPwrAmpPin() const;
    string getGpioBank() const;
    uint32_t getAmpGpioMask() const;
    uint32_t getAtrMasks() const;
    uint32_t getAtrControl() const;
    uint32_t getGpioDdr() const;
    int getRefOutInt() const;

    // USRP
    usrp::multi_usrp::sptr getUsrp() const;
    tx_streamer::sptr getTxStream() const;
    rx_streamer::sptr getRxStream() const;
    vector<string>& getTxChannelStrings();
    vector<size_t>& getTxChannelNums();
    vector<string>& getRxChannelStrings();
    vector<size_t>& getRxChannelNums();

  private:
    friend class SdrHwTest;
    void loadConfigFromYamlUsrp(const string& kYamlFile);
    void check10MhzLock();
    void gpsLock();
    void checkAndSetTime();
    void detectChannels();
    void setRFParams() override;
    void refLoLockDetect();
    void setupGpio();
    void setupTx() override;
    void setupRx() override;
    void storeTxChannel(size_t ch);
    void storeRxChannel(size_t ch);

    // GPIO
    int pwr_amp_pin;        // Which GPIO pin to use for external power amplifier control (set to -1 if not using)
    string gpio_bank;       // Which GPIO bank to use (FP0 is front panel and default)
    uint32_t AMP_GPIO_MASK;
    uint32_t ATR_MASKS;
    uint32_t ATR_CONTROL;
    uint32_t GPIO_DDR;
    int ref_out_int;        // Turns the 10 MHz reference out signal on (1) or off (0)
                            // set to (-1) if SDR does not support

    // USRP
    usrp::multi_usrp::sptr usrp;
    tx_streamer::sptr tx_stream;
    rx_streamer::sptr rx_stream;
    vector<string> tx_channel_strings;
    vector<size_t> tx_channel_nums;
    vector<string> rx_channel_strings;
    vector<size_t> rx_channel_nums;

};

#endif
