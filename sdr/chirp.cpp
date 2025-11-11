#include "chirp.hpp"

/**
 * @brief constructs a new Chirp object
 * 
 * creates new Chirp object and calls function that initializes each member variable from yaml file 
 * @param kYamlFile Path to the YAML configuration file (config/)
 */

Chirp::Chirp(const string& kYamlFile){
    assignVarFromYaml(kYamlFile);
}

/**
 * @brief assigns all member variables from YAML file
 * 
 * reads in paramaters from the YAML file in the config/ folder and assigns paramaters to variables in Chirp class
 * @param kYamlFile Path to the YAML configuration file (config/)
 */
void Chirp::assignVarFromYaml(const string& kYamlFile){
    YAML::Node config = YAML::LoadFile(kYamlFile);
    YAML::Node chirp = config["CHIRP"];

    time_offset = chirp["time_offset"].as<double>();
    tx_duration = chirp["tx_duration"].as<double>();
    rx_duration = chirp["rx_duration"].as<double>();
    tr_on_lead = chirp["tr_on_lead"].as<double>();
    tr_off_trail = chirp["tr_off_trail"].as<double>();
    pulse_rep_int = chirp["pulse_rep_int"].as<double>();
    tx_lead = chirp["tx_lead"].as<double>();
    num_pulses = chirp["num_pulses"].as<int>();
    num_presums = chirp["num_presums"].as<int>(1); // Default of 1 is equivalent to no pre-summing
    phase_dither = chirp["phase_dithering"].as<bool>(false);

    /**
    * sanity checks for Chirp class
    * 
    * performs sanity checks on the Chirp class to ensure that the parameters are valid
    */
      if (config["GENERATE"]["chirp_length"].as<double>() > tx_duration){
        cout << "WARNING: TX duration is shorter than chirp duration.\n";
     }
     if (config["CHIRP"]["rx_duration"].as<double>() < tx_duration) {
        cout << "WARNING: RX duration is shorter than TX duration.\n";
     }
}

double Chirp::getTimeOffset() const {return time_offset;}
double Chirp::getTxDuration() const {return tx_duration;}
double Chirp::getRxDuration() const {return rx_duration;}
double Chirp::getTrOnLead() const {return tr_on_lead;}
double Chirp::getTrOffTrail() const {return tr_off_trail;}
double Chirp::getPulseRepInt() const {return pulse_rep_int;}
double Chirp::getTxLead() const {return tx_lead;}
int Chirp::getNumPulses() const {return num_pulses;}
int Chirp::getNumPresums() const {return num_presums;}
bool Chirp::getPhaseDither() const {return phase_dither;}
int Chirp::getMaxChirpsPerFile() const {return max_chirps_per_file;}

void Chirp::setTimeOffset(double value) {
    if (value < 0.0) {
        throw invalid_argument("Time offset [s] must be greater than 0.");
    }
    time_offset = value;
}
void Chirp::setMaxChirpsPerFile(int value) {
    if (value == 0 || value < -1) {
        throw invalid_argument("max_chirps_per_file value must be greater than 0 or equal to -1.");
    }
    max_chirps_per_file = value;
}
