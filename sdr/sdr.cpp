#include "sdr.hpp"

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
* @brief initializes an SDR device.
*
* Virtual function that will initialize an SDR. Because this is an
*   abstract class, won't set anything up.
*
*/
void Sdr::createRadio(){
  cout << endl;
  cout << boost::format("Creating an undefinied radio: %s...")
    % device_args << endl;
}

/**
* @brief sets up the SDR
*
* Virtual function for the abstract class SDR; will be used to initialize
*   GPIO pins, Rx and Tx streams, and clocks on an actual SDR.
*/
void Sdr::setupRadio(){
  cout << endl;
  cout << boost::format("Setting up up an undefinied radio") << endl;
}



/*** @brief Sets the RF parameters for the SDR
 * 
 * Configures the RF parameters for given SDR device
*/
void Sdr::setRFParams(){
  cout << endl;
  cout << boost::format("Setting up RF parameters") << endl;
}


/*** @brief sets up a transmit for Sdr objects
 * 
 * Initializes the transmit stream depending on SDR in use
*/
void Sdr::setupTx(){
  cout << endl;
  cout << boost::format("Transmitting Method for an undefinied radio") << endl;
}

/*** @brief sets up a receiver for Sdr objects
 * 
 * Initializes the receive stream depending on SDR in use
 */
void Sdr::setupRx(){
  cout << endl;
  cout << boost::format("Receiving Method for an undefinied radio") << endl;
}

// DEVICE
string Sdr::getDeviceArgs() const {return device_args;}
string Sdr::getSubdev() const {return subdev;}
string Sdr::getClkRef() const {return clk_ref;}
double Sdr::getClkRate() const {return clk_rate;}
string Sdr::getTxChannels() const {return tx_channels;}
string Sdr::getRxChannels() const {return rx_channels;}
string Sdr::getCpuFormat() const {return cpu_format;}
string Sdr::getOtwFormat() const {return otw_format;}

// // GPIO
// int Sdr::getPwrAmpPin() const {return pwr_amp_pin;}
// string Sdr::getGpioBank() const {return gpio_bank;}
// uint32_t Sdr::getAmpGpioMask() const {return AMP_GPIO_MASK;}
// uint32_t Sdr::getAtrMasks() const {return ATR_MASKS;}
// uint32_t Sdr::getAtrControl() const {return ATR_CONTROL;}
// uint32_t Sdr::getGpioDdr() const {return GPIO_DDR;}
// int Sdr::getRefOutInt() const {return ref_out_int;}

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

// // USRP
// usrp::multi_usrp::sptr Sdr::getUsrp() const {return usrp;}
// tx_streamer::sptr Sdr::getTxStream() const {return tx_stream;}
// rx_streamer::sptr Sdr::getRxStream() const {return rx_stream;}
// vector<string>& Sdr::getTxChannelStrings() {return tx_channel_strings;}
// vector<size_t>& Sdr::getTxChannelNums() {return tx_channel_nums;}
// vector<string>& Sdr::getRxChannelStrings() {return rx_channel_strings;}
// vector<size_t>& Sdr::getRxChannelNums() {return rx_channel_nums;}
