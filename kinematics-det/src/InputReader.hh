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
  // InputReader();
  ~InputReader();
  static InputReader* Instance(std::string);
  static InputReader* Instance();

  void SetInstance(std::string fn){ s_instance = new InputReader(fn);}

  std::string GetFileOutput() {return filename_output;}
  std::string GetFileNucStates() {return filename_nuclear_states;}
  std::string GetFileMassTable() {return filename_mass_table;}
  std::string GetFileAngDis() {return filename_angular_distribution;}
  std::string GetFileMacro() {return filename_run_macro;}
  double GetBeamE() {return beam_energy;}
  double GetBeamEsimga() {return beam_energy_sigma;}
  int GetBeamZ() {return beam_Z;}
  int GetBeamA() {return beam_A;}

  static InputReader& GetInstance() {return *s_instance;}

private:
  // void SetFileOutput(std::string fn) {filename_output=fn;}
  // void SetFileNucStates(std::string fn) {filename_nuclear_states=fn;}
  // void SetFileMassTable(std::string fn) {filename_mass_table=fn;}
  // void SetFileAngDis(std::string fn) {filename_angular_distribution=fn;}
  // void SetFileMacro(std::string fn) {filename_run_macro=fn;}

private:
  static InputReader* s_instance;
  // InputReader* instance;

  // Values
  std::string filename_output;
  std::string filename_nuclear_states;
  std::string filename_mass_table;
  std::string filename_angular_distribution;
  std::string filename_run_macro;

  int beam_Z;
  int beam_A;
  double beam_energy;
  double beam_energy_sigma;


};

#endif