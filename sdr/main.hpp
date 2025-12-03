#pragma once

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
#include "pseudorandom_phase.hpp"
#include "utils.hpp"
#include "sdr.hpp"
#include "chirp.hpp"

#include "common.hpp"

constexpr const char* RADIO_TYPE = "@RADIO_TYPE@";

#ifdef RADIO_ETTUS
    #include "common_usrp.hpp"
    #include "hisnr_usrp.hpp"
    #include "usrp_rf_settings.hpp"
#elif defined(RADIO_AMD)
    #include "common_rfsoc42.hpp"
    #include "hisnr_rfsoc42.hpp"
#endif

bool checkForFullSampleSum(Chirp& chirp, vector<complex<float>>& sample_sum, ofstream& outfile);
void splitOutputFiles(Chirp& chirp, ofstream& outfile, string& current_filename, int& save_file_index);
void wrapUp(boost::asio::posix::stream_descriptor& gps_stream, ofstream& outfile, string& current_filename, boost::thread_group& transmit_thread);
