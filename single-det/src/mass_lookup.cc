
/*
mass_lookup
adopted from G. McCann's code in the MASK repository
but it's now using a json file
*/

#include "mass_lookup.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

MassLookup* MassLookup::s_instance = new MassLookup();
MassLookup::~MassLookup() {}

MassLookup::MassLookup() {
  std::ifstream mass_file("assets/nuclear_masses.json");

  KeyPair key;

  if (!mass_file.is_open()) {
    std::cerr << "Unable to load configuration in: files/nuclear_masses.json" << std::endl;
    exit(0);
  } else {
    std::cout << "Loaded mass file successful." << std::endl;
  }

  j_data = json::parse(mass_file);

  for (auto it : j_data.items()) {
    // std::cout << it.value()["A"] << std::endl;
    key.Z = it.value()["z"];
    key.A = it.value()["A"];
    elementTable[key.GetID()] = it.key();
    if (it.value()["isotope_mass"].is_null()) {
      massTable[key.GetID()] = -1.0;
    } else {
      massTable[key.GetID()] = it.value()["isotope_mass"];	
    }
  }
  std::cout << "Finished filling mass table."
}

// Returns nuclear mass in MeV
double MassLookup::FindMass(uint32_t Z, uint32_t A) {
  KeyPair key({Z, A});
  auto data = massTable.find(key.GetID());
  // if (data == massTable.end()) throw MassException();

  return data->second;
}

// returns element symbol
std::string MassLookup::FindSymbol(uint32_t Z, uint32_t A) {
  KeyPair key({Z, A});
  auto data = elementTable.find(key.GetID());
  // if (data == elementTable.end()) throw MassException();

  return data->second;
}
