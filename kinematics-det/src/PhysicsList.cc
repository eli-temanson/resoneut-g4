
#include "PhysicsList.hh"
#include "BinaryReactionPhysics.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
  G4int verb = 1;  
  SetVerboseLevel(verb);
  
  // RegisterPhysics(new G4EmStandardPhysics_option4(verb));
  pEmPhysicsList = new G4EmStandardPhysics(verb);
  pParticleList = new G4DecayPhysics(verb);
  pHadronPhys = new G4HadronPhysicsFTFP_BERT_HP(verb);
  // RegisterPhysics(new G4EmStandardPhysics(verb));
  // RegisterPhysics(new G4DecayPhysics(verb));
  // RegisterPhysics(new BinaryReactionPhysics(verb));
  // RegisterPhysics(new G4StepLimiterPhysics());
  // RegisterPhysics(new G4HadronPhysicsFTFP_BERT_HP(verb));
  // RegisterPhysics(new G4HadronElasticPhysicsHP(verb));

}

PhysicsList::~PhysicsList() {
  delete pParticleList;
  delete pEmPhysicsList;
  delete pHadronPhys;
}

void PhysicsList::ConstructParticle() {
  pParticleList->ConstructParticle();
}


void PhysicsList::ConstructProcess() {
  AddTransportation();

  pEmPhysicsList->ConstructProcess();
  pParticleList->ConstructProcess();

  BinaryReactionPhysics* rxnPhysics = new BinaryReactionPhysics();
  rxnPhysics->ConstructProcess();

  G4StepLimiterPhysics* stepper = new G4StepLimiterPhysics();
  stepper->ConstructProcess();

  pHadronPhys->ConstructProcess();
  G4HadronElasticPhysicsHP* elastic = new G4HadronElasticPhysicsHP();
  elastic->ConstructProcess();
}


void PhysicsList::SetCuts() {
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");
}
