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

#ifndef ScintSD_h
#define ScintSD_h 1

#include "G4VHit.hh"
#include "G4Allocator.hh"

class ScintHit : public G4VHit
{
public:
    ScintHit();
    ~ScintHit();
    
    void AddEdep(const G4double e) {edep += e;}
    G4double GetEdep() const {return edep;}
    
private:
    G4double edep;
};

#include "G4THitsCollection.hh"
typedef G4THitsCollection<ScintHit> ScintHitsCollection;

//===================================================================

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh"

class ScintSD : public G4VSensitiveDetector
{
public:
    ScintSD(G4String);
    ~ScintSD();
    
private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void Initialize(G4HCofThisEvent *);
    virtual void EndOfEvent(G4HCofThisEvent *);

    ScintHitsCollection* hitCollection;
};

#endif