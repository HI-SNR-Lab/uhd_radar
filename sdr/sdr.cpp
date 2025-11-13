#include "sdr.hpp"

using namespace RadioDeclaration;

/**
* @brief Throws error: it should be impossible to construct
*         an Sdr object without a parameter
*/
Sdr::Sdr() {
  cout << "Error: Impossible to construct Sdr without *.yaml file" << endl;
}

/**
* @brief Constructs a new Sdr object
*
* Constructs a new Sdr object and calls the relevant functions
* that initialize each member variable from the specified YAML file.
*
* @param kYamlFile Path to the YAML configuration file (config/)
*/
Sdr::Sdr(const string& kYamlFile) {
  loadConfigFromYaml(kYamlFile);
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
void Sdr::loadConfigFromYaml(const string& kYamlFile) {
  YAML::Node config = YAML::LoadFile(kYamlFile);

  // Device
  YAML::Node dev_params = config["DEVICE"];
  subdev = dev_params["subdev"].as<string>();
  clk_ref = dev_params["clk_ref"].as<string>();
  device_args = dev_params["device_args"].as<string>();
  clk_rate = dev_params["clk_rate"].as<double>();
  tx_channels = dev_params["tx_channels"].as<string>();
  rx_channels = dev_params["rx_channels"].as<string>();
  cpu_format = dev_params["cpu_format"].as<string>("fc32");
  otw_format = dev_params["otw_format"].as<string>();

  // // GPIO
  // YAML::Node gpio_params = config["GPIO"];
  // gpio_bank = gpio_params["gpio_bank"].as<string>();
  // pwr_amp_pin = gpio_params["pwr_amp_pin"].as<int>();
  // pwr_amp_pin -= 2; // map the specified DB15 pin to the GPIO pin numbering
  // if (pwr_amp_pin != -1) {
  //   AMP_GPIO_MASK = (1 << pwr_amp_pin);
  //   ATR_MASKS = (AMP_GPIO_MASK);
  //   ATR_CONTROL = (AMP_GPIO_MASK);
  //   GPIO_DDR = (AMP_GPIO_MASK);
  // }

  // ref_out_int = gpio_params["ref_out"].as<int>();

  // RF
  rf0 = config["RF0"];
  rf1 = config["RF1"];
  rx_rate = rf1["rx_rate"].as<double>();
  tx_rate = rf1["tx_rate"].as<double>();
  freq = rf1["freq"].as<double>();
  rx_gain = rf1["rx_gain"].as<double>();
  tx_gain = rf1["tx_gain"].as<double>();
  bw = rf1["bw"].as<double>();
  tx_ant = rf1["tx_ant"].as<string>();
  rx_ant = rf1["rx_ant"].as<string>();

  transmit = rf0["transmit"].as<bool>(true); // True if transmission enabled

/**
* Sanity checks for configuration parameters
*
* Ensures that the configuration parameters loaded from the YAML file
* are consistent and valid.
*
*/

  if (tx_rate != rx_rate){
    cout << "WARNING: TX sample rate does not match RX sample rate.\n";
  }
  if (config["GENERATE"]["sample_rate"].as<double>() != tx_rate){
    cout << "WARNING: TX sample rate does not match sample rate of generated chirp.\n";
  }
  if (bw < config["GENERATE"]["chirp_bandwidth"].as<double>() && bw != 0){
    cout << "WARNING: RX bandwidth is narrower than the chirp bandwidth.\n";
  }
}

/**
* @brief Initializes the USRP device setting time and clock source
*
* Creates the USRP device using the specified device arguments
* and sets the clock source to the specified reference clock.
* It also locks the mboard clocks and sets the time source.
* It ensures that the USRP device is ready for operation
*
*/
void Sdr::createRadio(){
  std::cout << boost::format("This is not a radio: specify radio in use") << endl;
}

/**
* @brief sets up the USRP device
*
* Initializes the USRP device with the specified parameters, sets the subdevice
* specifications, master clock rate, and configures the RF parameters. checks
* reference and LO locks, and initializes GPIO, transmit, and receive settings.
*
*/
void Sdr::setupRadio(){
  std::cout << boost::format("This is not a radio: specify radio in use") << endl;
}


void Sdr::setRFParams(){
 std::cout << boost::format("This is not a radio: specify radio in use") << endl;
}

/*** @brief Sets up the transmit stream for the USRP device
 * 
 * Initializes the transmit stream with the specified CPU and OTW formats,
 * and sets the number of channels for transmission. If transmission is enabled,
 * retrieves the transmit stream from the USRP device and prints maximum
 * number of samples that can be sent in a single call.
*/
void Sdr::setupTx(){
  std::cout << boost::format("This is not a radio: specify radio in use") << endl;
}

/*** @brief Sets up the receive stream for the USRP device
 * 
 * Initializes the receive stream with the specified CPU and OTW formats,
 * and sets the number of channels for reception. Retrieves the receive stream
 * from the USRP device and prints maximum number of samples that can be
 * received in a single call.
 */
void Sdr::setupRx(){
   std::cout << boost::format("This is not a radio: specify radio in use") << endl;
}

// DEVICE
string Sdr::getDeviceArgs() const {return device_args;}
string Sdr::getType() const {return type;}
string Sdr::getSubdev() const {return subdev;}
string Sdr::getClkRef() const {return clk_ref;}
double Sdr::getClkRate() const {return clk_rate;}
string Sdr::getTxChannels() const {return tx_channels;}
string Sdr::getRxChannels() const {return rx_channels;}
string Sdr::getCpuFormat() const {return cpu_format;}
string Sdr::getOtwFormat() const {return otw_format;}

// RF
YAML::Node Sdr::getRf0() const {return rf0;}
YAML::Node Sdr::getRf1() const {return rf1;}
double Sdr::getRxRate() const {return rx_rate;}
double Sdr::getTxRate() const {return tx_rate;}
double Sdr::getFreq() const {return freq;}
double Sdr::getRxGain() const {return rx_gain;}
double Sdr::getTxGain() const {return tx_gain;}
double Sdr::getBw() const {return bw;}
string Sdr::getRxAnt() const {return rx_ant;}
string Sdr::getTxAnt() const {return tx_ant;}
bool Sdr::getTransmit() const {return transmit;}

