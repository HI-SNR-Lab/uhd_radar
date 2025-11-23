#ifndef MAIN_RFSOC42_SETTINGS_HPP
#define MAIN_RFSOC42_SETTINGS_HPP

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
#include "usrp_rf_settings.hpp"
#include "pseudorandom_phase.hpp"
#include "utils.hpp"
#include "sdr.hpp"
#include "chirp.hpp"
#include "common.hpp"

bool checkForFullSampleSum(Chirp& chirp, vector<complex<float>>& sample_sum, ofstream& outfile);
void splitOutputFiles(Chirp& chirp, ofstream& outfile, string& current_filename, int& save_file_index);
void wrapUp(boost::asio::posix::stream_descriptor& gps_stream, ofstream& outfile, string& current_filename, boost::thread_group& transmit_thread);

#endif