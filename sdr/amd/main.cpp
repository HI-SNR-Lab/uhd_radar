#include "main.hpp"

/*
 * SIG INT HANDLER
 */
static bool stop_signal_called = false;
void sig_int_handler(int) {
  stop_signal_called = true;
}

/*** CONFIGURATION PARAMETERS ***/

// FILENAMES
string chirp_loc;
string output_dir;
string save_loc;
string gps_save_loc;

// Calculated Parameters
double tr_off_delay; // Time before turning off GPIO
size_t num_tx_samps; // Total samples to transmit per chirp
size_t num_rx_samps; // Total samples to receive per chirp

// Global state
long int pulses_scheduled = 0;
long int pulses_received = 0;
long int error_count = 0;
long int last_pulse_num_written = -1; // Index number (pulses_received - error_count) of last sample written to outfile

// Cout mutex
std::mutex cout_mutex;

/**
 * @brief Writes received RX data to file if enough pulses have been received
 * 
 * Checks if the number of pulses received is enough to write a full sample_sum to the file, only if enough error-free pulses have been received.
 * @param pulses_received Total number of pulses received
 * @param error_count Total number of errors encountered
 * @param last_pulse_num_written Last pulse number written to the file
 * @param chirp Chirp object containing parameters for the chirp
 * @param sample_sum Vector containing the sum of error-free RX pulses
 * @param outfile Output file stream to write the RX data
 * @return Returns true if the data was successfully written to the file, false otherwise signaling error
 */
bool checkForFullSampleSum(Chirp& chirp, vector<complex<float>>& sample_sum, ofstream& outfile) {
  if (((pulses_received - error_count) > last_pulse_num_written) && ((pulses_received - error_count) % chirp.getNumPresums() == 0)) {
    // As each sample is added, it has phase inversion applied and is divided by # presums, so no additional work to do here.
    // write RX data to file
    if (outfile.is_open()) {
      outfile.write((const char*)&sample_sum.front(), 
        num_rx_samps * sizeof(complex<float>));
    } else {
      cout_mutex.lock();
      cout << "Cannot write to outfile!" << endl;
      cout_mutex.unlock();
      return false; // Error writing to file
    }
    fill(sample_sum.begin(), sample_sum.end(), complex<float>(0,0)); // Zero out sum for next time
    last_pulse_num_written = pulses_received - error_count;
  }
  return true;
}

// Split output files based on number of chirps

/**
 * @brief Determines if the amount of pulses received is enough to add another file for storage
 * 
 * Creates more files if the number of pulses is higher than the maximum number of chirps per file, but only if file splitting is enabled.
 * @param chirp Chirp object containing parameters for the chirp
 * @param outfile Output file stream to write the RX data
 * @param current_filename Current filename for the output file
 * @param save_file_index Index of the current file being saved
 * @param last_pulse_num_written Last pulse number written to the file
 */
void splitOutputFiles(Chirp& chirp, ofstream& outfile, string& current_filename, int& save_file_index) {
  if ( (chirp.getMaxChirpsPerFile() > 0) && (int(last_pulse_num_written / chirp.getMaxChirpsPerFile()) > save_file_index)) {
    outfile.close();
    // Note: This print statement is used by automated post-processing code. Please be careful about changing the format.
    cout_mutex.lock();
    cout << "[CLOSE FILE] " << current_filename << endl;
    cout_mutex.unlock();

    save_file_index++;
    current_filename = save_loc + "." + to_string(save_file_index);

    // Note: This print statement is used by automated post-processing code. Please be careful about changing the format.
    cout_mutex.lock();
    cout << "[OPEN FILE] " << current_filename << endl;
    cout_mutex.unlock();
    outfile.open(current_filename, ofstream::binary);
  }
}

//Wrapping up main function after the RX loop is done

/**
 * @brief Finalizes the main function once the main while loop is done
 * 
 * Various tasks are finished and significant information is printed to the console, such as the number of errors encountered, total pulses written, and total pulses attempted.
 * @param gps_stream GPS stream descriptor for closing the GPS file
 * @param outfile Output file stream to close
 * @param current_filename Current filename for the output file
 * @param error_count Total number of errors encountered during the RX process
 * @param last_pulse_num_written Last pulse number written to the file
 * @param pulses_received Total number of pulses received during the RX process
 * @param transmit_thread Thread group for the transmit worker
 */
void wrapUp(boost::asio::posix::stream_descriptor& gps_stream, ofstream& outfile, string& current_filename, boost::thread_group& transmit_thread) {
  cout << "[RX] Closing output file." << endl;
  outfile.close();
  cout << "[CLOSE FILE] " << current_filename << endl;

  gps_stream.close();

  cout << "[RX] Error count: " << error_count << endl;
  cout << "[RX] Total pulses written: " << last_pulse_num_written << endl;
  cout << "[RX] Total pulses attempted: " << pulses_received << endl;
  
  cout << "[RX] Done. Calling join_all() on transmit thread group." << endl;

  transmit_thread.join_all();

  cout << "[RX] transmit_thread.join_all() complete." << endl << endl;
}

int _rfsoc42_main(YAML::Node config, string yaml_filename) {
  #include "hisnr_rfsoc42.hpp"
  return EXIT_SUCCESS;
}

/* 
 * UHD_SAFE_MAIN
 */
int UHD_SAFE_MAIN(int argc, char *argv[]) {

  /** Load radar mode */
  string radar_mode;

  /** Load YAML file **/
  string yaml_filename;

  // determines yaml and radar_mode (if specified)
  if (argc >= 2) {
    yaml_filename = "../../" + string(argv[1]);
  } else {
    yaml_filename = "../../config/default.yaml";
  }
  cout << "Reading from config file: " << yaml_filename << endl;

  return EXIT_SUCCESS;
  
}
