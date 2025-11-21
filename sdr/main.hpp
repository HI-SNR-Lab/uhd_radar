#pragma once

#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <uhd/convert.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/barrier.hpp>
#include <fstream>
#include <csignal>
#include <complex>
#include <mutex>
#include <cstdlib>
#include <filesystem>
#include <boost/asio/io_service.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/write.hpp>

#include "yaml-cpp/yaml.h"
#include "usrp_rf_settings.hpp"
#include "pseudorandom_phase.hpp"
#include "utils.hpp"
#include "sdr.hpp"
#include "hisnr_usrp.hpp"
#include "hisnr_rfsoc42.hpp"
#include "chirp.hpp"
#include "common.hpp"

int _usrp_main(YAML::Node config, string yaml_filename);
int _rfsoc42_main(YAML::Node config, string yaml_filename);

// Constants that are used to determine type of SDR
extern const std::string RADIO_TYPE_B210 = "ettus_b2x0";
extern const std::string RADIO_TYPE_RFSOC42 = "xilinx_rfsoc4x2";

void transmit_worker(tx_streamer::sptr& tx_stream, rx_streamer::sptr& rx_stream, Chirp& chirp, Sdr& sdr);
void handleRxBuffer(size_t n_samps_in_rx_buff, rx_metadata_t& rx_md, Chirp& chirp, vector<complex<float>>& buff, vector<complex<float>>& sample_sum, float& inversion_phase);
bool checkForFullSampleSum(Chirp& chirp, vector<complex<float>>& sample_sum, ofstream& outfile);
void splitOutputFiles(Chirp& chirp, ofstream& outfile, string& current_filename, int& save_file_index);
void wrapUp(boost::asio::posix::stream_descriptor& gps_stream, ofstream& outfile, string& current_filename, boost::thread_group& transmit_thread);
