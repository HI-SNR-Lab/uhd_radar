#ifndef CHIRP_HPP
#define CHIRP_HPP
#include "yaml-cpp/yaml.h"
#include "common.hpp"

class Chirp{

public:
    Chirp(const string& kYamlFile);

    double getTimeOffset() const;
    void setTimeOffset(double value);
    double getTxDuration() const;
    double getRxDuration() const;
    double getTrOnLead() const;
    double getTrOffTrail() const;
    double getPulseRepInt() const;
    double getTxLead() const;
    int getNumPulses() const;
    int getNumPresums() const;
    bool getPhaseDither() const;
    int getMaxChirpsPerFile() const;
    void setMaxChirpsPerFile(int value);

private:
    void assignVarFromYaml(const string& kYamlFile);

    double time_offset;      // [s] Offset time before the first received sample
    double tx_duration;      // [s] Transmission duration
    double rx_duration;      // [s] Receive duration
    double tr_on_lead;       // [s] Time from GPIO output toggle on to TX (if using GPIO)
    double tr_off_trail;     // [s] Time from TX off to GPIO output off (if using GPIO)
    double pulse_rep_int;    // [s] Pulse period
    double tx_lead;          // [s] Time between start of TX and RX
    int num_pulses;          // No. of chirps to TX/RX - set to -1 to continuously transmit pulses until stopped
    int num_presums;         // Number of received pulses to average over before writing to file
    bool phase_dither;       // Enable phase dithering
    int max_chirps_per_file; // Maximum number of RX from a chirp to write to a single file set to -1 to avoid breaking
                             // into multiple files
};

#endif
