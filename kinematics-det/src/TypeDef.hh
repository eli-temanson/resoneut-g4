#ifndef TYPEDEF_H
#define TYPEDEF_H

#define ASSERT_WITH_MESSAGE(condition, message) \
do { \
  if(!(condition)) { \
    printf((message)); \
  } \
  assert((condition)); \
} while(false) \

typedef struct theshold_struct {
  double energy;
  uint decay_charge;
  uint decay_mass;
} threshold_struct;

typedef struct state_struct {
  double energy;
  uint spin2;
  bool parity;
  double probability;
} state_struct;

typedef struct isotope_struct {
  std::string name;
  uint charge;
  uint mass;
  std::vector<state_struct> states;
  std::vector<threshold_struct> thresholds;
} isotope_struct;

#include "G4LorentzVector.hh"
#include "G4ThreeVector.hh"
typedef struct kinematics_struct {
  std::string name;
  double M,InvMass,KE,E,Ecm,Ex;
  double x, y, z;
  double Theta,Phi;
  double P;
  G4LorentzVector LV;
  G4ThreeVector Pos;
  int A, Z;
} kinematics_struct;


#endif //TYPEDEF_H