#ifndef HISNR_RFSOC42_HPP
#define HISNR_RFSOC42_HPP

#include "yaml-cpp/yaml.h"
#include "common.hpp"
#include "sdr.hpp"
#include "pybind11/pybind11.h"

namespace py = pybind11;

class HiSnrRFSoC42 : public Sdr {

  public:
    HiSnrRFSoC42() = delete;
    HiSnrRFSoC42(const string& kYamlFile);

    void createRadio() override;
    void setupRadio() override;

    //RFSoC4x2
    py::module_ getBaseOverlay() const;
    py::object getRadio() const;
    py::object getRFSoC() const;

  private:
    friend class SdrHwTest;
    void loadConfigFromYamlRFSoC(const string& kYamlFile);
    void checkAndSetTime();
    void setRFParams() override;
    void setupTx() override;
    void setupRx() override;
    void storeTxChannel(size_t ch);
    void storeRxChannel(size_t ch);


    py::module_ base_overlay;
    py::object base;
    py::object radio;

};

#endif // HISNR_RFSOC42_HPP
