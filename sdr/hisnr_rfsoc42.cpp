#include "hisnr_rfsoc42.hpp"

namespace py = pybind11;

/**
* @brief Constructs a new HiSnrRFSoC42 object
*
* Constructs a new HiSnrRFSoC42 object and calls the relevant functions
* that initialize each member variable from the specified YAML file.
*
* @param kYamlFile Path to the YAML configuration file (config/)
*/
HiSnrRFSoC42::HiSnrRFSoC42(const string& kYamlFile) : Sdr(kYamlFile) {
  
  py::module_ overlays = py::module_::import("pynq.overlays.base");
  base_overlay = overlays.attr("BaseOverlay");
  base = base_overlay("base.bit");
  radio = base.attr("radio"); // equivalent of pynq's base.radio

  loadConfigFromYamlRFSoC(kYamlFile);
}

/**
* @brief Load configuration from YAML file into the SDR class
*
* Reads in SDR and hardware-related configuration settings from the
* provided YAML file, and stores them in the corresponding member
* variables of the SDR class.
*
* @param kYamlFile Path to the YAML configuration file (config/)
*/
void HiSnrRFSoC42::loadConfigFromYamlRFSoC(const string& kYamlFile) {
  YAML::Node config = YAML::LoadFile(kYamlFile);
}

/**
* @brief Initializes the RFSoC42 device setting time and clock source
*
* Creates the RFSoC42 device using the specified device arguments
* and sets the clock source to the specified reference clock.
* It also locks the mboard clocks and sets the time source.
* It ensures that the RFSoC42 device is ready for operation
*
*/
void HiSnrRFSoC42::createRadio(){
  py::object b = getRFSoC(); // base
  py::object r = getRadio(); // base.radio
  b.attr("init_rf_clks"); // initializes the RF Clocks on the board

  py::module_ builtins = py::module_::import("builtins");
  cout << boost::format("Radio Wrapper Info: %s") % string(py::str(builtins.attr("help")(r))) << endl;
  py::object receiver = r.attr("receiver"); // pynq.base.radio.receiver
  py::object getchanneldescription = receiver.attr("get_channel_description");
  cout << boost::format("Radio Channel Information: %s") % string(py::str(getchanneldescription)) << endl;
}

/**
* @brief sets up the RFSoC42 device
*
* Initializes the RFSoC42 device with the specified parameters, sets the subdevice
* specifications, master clock rate, and configures the RF parameters. checks
* reference and LO locks, and initializes GPIO, transmit, and receive settings.
*
*/
void HiSnrRFSoC42::setupRadio(){
  
}

/*** @brief Checks the RFSoC42 time against GPS time
 *
 */
void HiSnrRFSoC42::checkAndSetTime(){
  
}

/*** @brief Sets the RF parameters for the RFSoC42 device
 * 
 * Configures the RF parameters for the RFSoC42 device based on the number of
 * TX channels specified. If only one TX channel is specified, calls
 * `set_rf_params_single` function. If two TX channels specified, calls
 * `set_rf_params_multi`. If number of channels is not supported, throws a runtime
 * error. After setting RF parameters, allows for a setup time
 * to ensure the parameters are applied correctly.
*/
void HiSnrRFSoC42::setRFParams(){
  
}


/*** @brief Sets up the transmit stream for the RFSoC42 device
 * 
 * Initializes the transmit stream with the specified CPU and OTW formats,
 * and sets the number of channels for transmission. If transmission is enabled,
 * retrieves the transmit stream from the RFSoC42 device and prints maximum
 * number of samples that can be sent in a single call.
*/
void HiSnrRFSoC42::setupTx(){
}

/*** @brief Sets up the receive stream for the RFSoC42 device
 * 
 * Initializes the receive stream with the specified CPU and OTW formats,
 * and sets the number of channels for reception. Retrieves the receive stream
 * from the RFSoC42 device and prints maximum number of samples that can be
 * received in a single call.
 */
void HiSnrRFSoC42::setupRx(){
}

//RFSoC
py::module_ HiSnrRFSoC42::getBaseOverlay() const {return base_overlay;}
py::object HiSnrRFSoC42::getRadio() const {return radio;}
py::object HiSnrRFSoC42::getRFSoC() const {return base;}
