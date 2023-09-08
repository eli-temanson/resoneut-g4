
#include "PhysicsList.hh"
#include "BinaryReactionPhysics.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"

// #include "G4HadronElasticPhysics.hh"
// #include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"

// #include "G4HadronPhysicsShieldingLEND.hh"
// #include "G4HadronElasticPhysicsLEND.hh"

#include "G4IonPhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4StoppingPhysics.hh"

// #include "Shielding.hh"
// #include "G4NeutronHPElastic.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
  G4int verb = 0;  
  SetVerboseLevel(verb);
  
  // RegisterPhysics(new G4EmStandardPhysics_option4(verb));
  pEmPhysicsList = new G4EmStandardPhysics(verb);
  pParticleList = new G4DecayPhysics(verb);
  pHadronPhys = new G4HadronPhysicsFTFP_BERT_HP(verb);  
  // pHadronPhys = new G4HadronPhysicsQGSP_BERT_HP(verb);

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

  G4HadronElasticPhysicsHP* elasticHP = new G4HadronElasticPhysicsHP();
  elasticHP->ConstructProcess();

  // G4HadronElasticPhysicsLEND* elasticLEND = new G4HadronElasticPhysicsLEND();
  // elasticLEND->ConstructProcess();
  // G4HadronPhysicsShieldingLEND* shieldLEND = new G4HadronPhysicsShieldingLEND();
  // shieldLEND->ConstructProcess();

  G4IonPhysics* ionPhysics = new G4IonPhysics();
  ionPhysics->ConstructProcess();  

  G4IonElasticPhysics* ionElasticPhysics = new G4IonElasticPhysics();
  ionElasticPhysics->ConstructProcess();  

  G4StoppingPhysics* stoppingPhysics = new G4StoppingPhysics();
  stoppingPhysics->ConstructProcess();  

  // G4NeutronHPElastic* neutronPhysics = new G4NeutronHPElastic();
  // neutronPhysics->ConstructProcess();

  // Shielding* shielding = new Shielding();
  // shielding->ConstructProcess();

}


void PhysicsList::SetCuts() {
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");
  SetCutValue(0.00*mm, "proton");  
  // G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(70.0*keV, 1.0*MeV);
}
