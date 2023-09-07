// Author: Eli Temanson
//

#ifndef mass_table_hpp
#define mass_table_hpp

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class MassLookup {
 public:
  // szudzik pairing method to make unqiue key out of two unsigned integers
  struct KeyPair {
    uint32_t Z;
    uint32_t A;

    std::size_t GetID() { return Z >= A ? Z * Z + Z + A : A * A + Z; }
  };

  ~MassLookup();
  double FindMass(uint32_t Z, uint32_t A);
  double FindMassU(uint32_t Z, uint32_t A) { return FindMass(Z, A) / u_to_mev; }
  std::string FindSymbol(uint32_t Z, uint32_t A);

  static MassLookup& GetInstance() { return *s_instance; }

 private:
  MassLookup();

  json j_data;

  static MassLookup* s_instance;
  std::unordered_map<std::size_t, double> massTable;
  std::unordered_map<std::size_t, std::string> elementTable;

  // constants
  static constexpr double u_to_mev = 931.4940954;
  static constexpr double electron_mass = 0.000548579909;
};

#endif