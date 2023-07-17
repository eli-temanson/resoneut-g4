

#include "PhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"
// binary reaction
#include "BinaryReactionPhysics.hh"

// #include "G4DecayPhysics.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4UnitsTable.hh"
// #include "G4NuclideTable.hh"


PhysicsList::PhysicsList() {
  G4int verb = 1;  
  SetVerboseLevel(verb);
  
  // Initalize the electromagnetic physics
  RegisterPhysics(new G4EmStandardPhysics_option4(verb));
  RegisterPhysics(new BinaryReactionPhysics(verb));
}

PhysicsList::~PhysicsList() {}


