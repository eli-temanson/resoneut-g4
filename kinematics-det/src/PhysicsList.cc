

#include "PhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4StoppingPhysics.hh"

#include "BinaryReactionPhysics.hh"

PhysicsList::PhysicsList() {
  G4int verb = 1;  
  SetVerboseLevel(verb);
  
  RegisterPhysics(new G4EmStandardPhysics_option4(verb));
  RegisterPhysics(new BinaryReactionPhysics(verb));
}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts() {
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");

}
