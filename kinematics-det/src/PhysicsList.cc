

#include "PhysicsList.hh"
// #include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh`"
#include "G4StepLimiterPhysics.hh"
#include "BinaryReactionPhysics.hh"

PhysicsList::PhysicsList() {
  G4int verb = 1;  
  SetVerboseLevel(verb);
  
  // RegisterPhysics(new G4EmStandardPhysics_option4(verb));
  RegisterPhysics(new G4EmStandardPhysics(verb));
  RegisterPhysics(new BinaryReactionPhysics(verb));
  RegisterPhysics(new G4StepLimiterPhysics());
  RegisterPhysics(new G4DecayPhysics(verb));
  // RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts() {
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");
}
