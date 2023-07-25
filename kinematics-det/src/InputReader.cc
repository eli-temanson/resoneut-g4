
#ifndef InputReader_cc
#define InputReader_cc

#include "InputReader.hh"

InputReader::InputReader(std::string input_filename) {

  // Read and parse json file.
  std::ifstream input_file(input_filename);
  json config = json::parse(input_file);
  input_file.close();
  
  std::cout << "Worked!" << std::endl;

}

InputReader::~InputReader() {}


#endif