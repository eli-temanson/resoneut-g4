#ifndef InputReader_h
#define InputReader_h

#include <string>
#include <fstream>
#include <iostream>

// using json-c++ reader
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class InputReader {

public:
  InputReader(std::string);
  ~InputReader();

  // Get()

private:
  // Set()

private:
  // Values
  std::string filename_output;
  std::string filename_nuclear_states;
  std::string filename_mass_table;
  std::string filename_angular_distribution;

  int beam_A;
  int beam_Z;
  double beam_energy;
  double beam_energy_sigma;


};

#endif