//===================================================================
//  Primary Generator Action for Geant4 Simulation
//
//  The purpose of this class is to initialize a partice gun. The 
//    type, direction, energy and randomization can all be setup in 
//    the "GeneratePrimaries" function. This class is class from the 
//    ActionInitialization class using, 
//      > "SetUserAction(new PrimaryGeneratorAction)"
//
//  A future use of this class is setting up 2-body kinematics and 
//    3-body breakup for reaction reconstruction.
//
//  Date of Last Modified: 12/15/2021
//  Author: Eli Temanson
//===================================================================

#include "PrimaryGeneratorAction.hh"
#include "InputReader.hh"

#include "G4GeneralParticleSource.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() : 
  G4VUserPrimaryGeneratorAction(), 
  pParticleGun(nullptr),
  pParticleSource(nullptr)
{
  pParticleSource = new G4GeneralParticleSource();
  pParticleGun = new G4ParticleGun(1);

  // // default particle kinematic
  // //
  // G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // G4ParticleDefinition* particle = particleTable->FindParticle("proton");
  // pParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  // pParticleGun->SetParticleEnergy(2.0 * MeV);
  // pParticleGun->SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() 
{ 
  delete pParticleGun; 
  delete pParticleSource;
}

// This function is called at the begining of each event
//
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // default particle kinematic
  //
  //G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //G4String particleName = "gamma";
  //G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
  //
  //pParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  //pParticleGun->SetParticleEnergy(59. * keV);
  //pParticleGun->SetParticleDefinition(particle);

  // G4double envSizeXY = 50.0*cm;
  // G4double envSizeZ = 1.0*cm;
  // G4double size = 0.5; 

  // Set gun position
  // //
  // pParticleGun->SetParticlePosition(G4ThreeVector( (G4UniformRand()*(50.0-20.0) - 20.0)*mm,
  //                                                  (G4UniformRand()*(50.0-20.0) - 20.0)*mm,
  //                                                 -1.0 * envSizeZ));
  // pParticleGun->SetParticlePosition(G4ThreeVector(size * envSizeXY * (G4UniformRand()-0.5),
  //                                                 size * envSizeXY * (G4UniformRand()-0.5),
  //                                                 -1.0 * envSizeZ));

  // pParticleGun->GeneratePrimaryVertex(anEvent);

  InputReader* reader = InputReader::Instance();
  G4IonTable* particleTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* particle = 
    particleTable->GetIon(reader->GetBeamZ(),reader->GetBeamA(),0.0);

  pParticleGun->SetParticlePosition(
    G4ThreeVector(
      G4RandGauss::shoot(-.0*mm,2.0*mm),
      G4RandGauss::shoot(-.0*mm,2.0*mm),
      -1.0*cm));

  pParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
  pParticleGun->SetParticleEnergy(
    G4RandGauss::shoot(reader->GetBeamE()*MeV, reader->GetBeamEsimga()*MeV));
  pParticleGun->SetParticleDefinition(particle);
  pParticleGun->GeneratePrimaryVertex(anEvent);
}


