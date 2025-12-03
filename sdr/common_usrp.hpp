#pragma once

#ifndef USRP_BOARD
#define USRP_BOARD

#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <uhd/convert.hpp>

#include "usrp_rf_settings.hpp"
#include "chirp.hpp"
#include "sdr.hpp"
#include "hisnr_usrp.hpp"

using namespace uhd;

void transmit_worker(tx_streamer::sptr& tx_stream, rx_streamer::sptr& rx_stream, Chirp& chirp, Sdr& sdr);
void handleRxBuffer(size_t n_samps_in_rx_buff, rx_metadata_t& rx_md, Chirp& chirp, vector<complex<float>>& buff, vector<complex<float>>& sample_sum, float& inversion_phase);

#endif
