#ifndef HISNR_RFSOC42_HPP
#define HISNR_RFSOC42_HPP

#include "yaml-cpp/yaml.h"
#include "usrp_rf_settings.hpp"
#include "common.hpp"
#include "sdr.hpp"

class HiSnrRFSoC42 : public Sdr {

  public:
    HiSnrRFSoC42() = delete;
    HiSnrRFSoC42(const string& kYamlFile);

    void createRadio() override;
    void setupRadio() override;

  private:
    friend class SdrHwTest;
    void loadConfigFromYamlRFSoC(const string& kYamlFile);
    void checkAndSetTime();
    void setRFParams() override;
    void setupTx() override;
    void setupRx() override;
    void storeTxChannel(size_t ch);
    void storeRxChannel(size_t ch);

};

#endif // HISNR_RFSOC42_HPP
