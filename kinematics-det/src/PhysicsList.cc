

#include "PhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "BinaryReactionPhysics.hh"

PhysicsList::PhysicsList() {
  G4int verb = 1;  
  SetVerboseLevel(verb);
  
  // Initalize the electromagnetic physics
  RegisterPhysics(new G4EmStandardPhysics_option4(verb));
  RegisterPhysics(new BinaryReactionPhysics(verb));
}

PhysicsList::~PhysicsList() {}


