//===================================================================
//  Sensitive Detector Class for Geant4 Simulation
//
//  This Class allows for a simple and convient way to extract 
//      simulation data for a given detector. This method is nice 
//      for single detector volumes but for larger project the Action
//      Event and Stepping classes should be utilized.
//
//  To implement this class make sure you have added the 
//      "ConstructSDandField" function to the Detector Construction.
//
//  Class includes: 
//      > Initialize()
//      > ProcessHits()
//          > GetTotalEnergyDeposit()
//      > EndOfEvent()
//
//  Date of Last Modified: 12/15/2021
//  Author: Eli Temanson
//===================================================================

#ifndef GenericSD_h
#define GenericSD_h 1

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4ParticleDefinition.hh"

//===================================================================
// GenericHit class.
//
class GenericHit : public G4VHit
{
public:
  GenericHit();
  // GenericHit(G4int, G4int, G4double, G4double, G4ThreeVector, G4ParticleDefinition*);
  ~GenericHit();

  void SetHit(G4int copyNo, G4int trackID, G4double time, G4double energy, G4ThreeVector pos, G4ParticleDefinition* particle) {
    ID = copyNo;
    TrackID = trackID;
    Time = time;
    Energy = energy;
    Position = pos;
    Particle = particle;
  }

  G4int GetID() const {return ID;}
  G4int GetTrackID() const {return TrackID;}
  G4double GetTime() const {return Time;}
  G4double GetEnergy() const {return Energy;}
  G4ThreeVector GetPosition() const {return Position;}
  G4ParticleDefinition* GetParticle() const {return Particle;}
  void Print();
  // inline void* operator new(size_t);
  // inline void operator delete(void*);

private:
  G4int ID;
  G4int TrackID;
  G4double Time;
  G4double Energy;
  G4ThreeVector Position;
  G4ParticleDefinition* Particle;
};

//===================================================================
// GenericHit definitions.
//
#include "G4THitsCollection.hh"
typedef G4THitsCollection<GenericHit> GenericHitsCollection;

// extern G4ThreadLocal G4Allocator<GenericHit>* GenericHitAllocator;
// inline void* GenericHit::operator new(size_t) {
//   if (!GenericHitAllocator) GenericHitAllocator = new G4Allocator<GenericHit>;
//   return (void*)GenericHitAllocator->MallocSingle();
// }
// inline void GenericHit::operator delete(void* aHit) {
//   GenericHitAllocator->FreeSingle((GenericHit*) aHit);
// }


//===================================================================
// GenericSD class.
//
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

class GenericSD : public G4VSensitiveDetector
{
public:
  GenericSD(G4String);
  ~GenericSD();

private:
  virtual void Initialize(G4HCofThisEvent *);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  virtual void EndOfEvent(G4HCofThisEvent *);
  
  GenericHitsCollection* hitCollection;
  G4int hcid;
};

#endif
