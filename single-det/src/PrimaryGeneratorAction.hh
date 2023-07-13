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


#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"


// The primary generator action class with particle gun.
//
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    virtual ~PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    //const G4ParticleGun* GetParticleGun() const { return pParticleGun; }
  
  private:
    G4ParticleGun*  pParticleGun; // pointer a to G4 gun class
};


#endif
