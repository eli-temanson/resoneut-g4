
#ifndef InputReader_cc
#define InputReader_cc

#include "InputReader.hh"

InputReader* InputReader::s_instance = nullptr; //new InputReader();

InputReader::~InputReader() {}

InputReader* InputReader::Instance() {
  if(!s_instance) {
    std::cerr << "No input reader instance has been initiated!\n";
    exit(3);
  }
  return s_instance;
}

InputReader* InputReader::Instance(std::string filename) {
  if(!s_instance) {
    s_instance = new InputReader(filename);
  }
  return s_instance;
}

// InputReader::InputReader() {}
InputReader::InputReader(std::string input_filename) {

  // Read and parse json file.
  std::ifstream input_file(input_filename);
  json config = json::parse(input_file);
  input_file.close();
  
  std::cout << "Processing the Input!" << std::endl;
  filename_output = config["filename_output"].get<std::string>();
  filename_nuclear_states = config["filename_nuclear_states"].get<std::string>();
  filename_mass_table = config["filename_mass_table"].get<std::string>();
  filename_angular_distribution = config["filename_angular_distribution"].get<std::string>();
  filename_run_macro = config["filename_run_macro"].get<std::string>();

  beam_energy = config["beam"][0]["energyMeV"].get<double>();
  beam_energy_sigma = config["beam"][0]["sigmaMeV"].get<double>();
  beam_Z = config["beam"][0]["ZA"][0].get<uint>();
  beam_A = config["beam"][0]["ZA"][1].get<uint>();

}



#endif