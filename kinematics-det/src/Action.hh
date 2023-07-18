//===================================================================
//  Action Class for Geant4 Simulation
//
//  This file contains the all Action classes required by Geant4. 
//  The Action Classes control what data, accessed at what level 
//    is saved to what data structure. The Run, Event and Stepping
//    Actions are initalized inside the following class,
// 
//      > ActionInitialization
//
//  In the Action Classes below, I am using Root-Cern data structures
//    initialized in the RunAction Class and the using the 
//    Sensitive Detector Class to fill my date tree.
//
//      > Action Run
//
//  The Action Event and Stepping are not currently being used
//    but have the ability to extract all necessary simulation data 
//    which requires more programming. See the following classes,
//    
//      > Event Action
//      > Stepping Action
//
//  Date of Last Modified: 12/15/2021
//  Author: Eli Temanson
//===================================================================

#ifndef Action_h
#define Action_h 1

#include "G4GenericAnalysisManager.hh"

using G4AnalysisManager = G4GenericAnalysisManager;

//===================================================================
// Action Initialization class.
//
#include "G4VUserActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};


//===================================================================
// Headers for Run Action Class
//
#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction();
    
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};


//===================================================================
// Headers for Event Action Class
//
#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "G4SDManager.hh"
#include "GenericSD.hh"

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction();
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
    // Set Values functions here.
    void AddEnergy(G4double e) {Energy += e;}
    void SetEnergy(G4double energy) {Energy = energy;}
    void SetCMEnergy(G4double cmEnergy) {CMEnergy = cmEnergy;}
    void SetVertexZ(G4double vertexZ) {VertexZ = vertexZ;}
    void SetQValue(G4double qValue) {QValue = qValue;}
    void SetExcitedEnergy(G4double excitedEnergy) {ExcitedEnergy = excitedEnergy;}
    void SetLightAngleCM(G4double angle) {LightAngleCM = angle;}
    void SetLightAngleLab(G4double angle) {LightAngleLab = angle;}
    void SetLightEnergy(G4double energy) {LightEnergy = energy;}
    void SetHeavyAngleCM(G4double angle) {HeavyAngleCM = angle;}
    void SetHeavyAngleLab(G4double angle) {HeavyAngleLab = angle;}
    void SetHeavyEnergy(G4double energy) {HeavyEnergy = energy;}
    void SetLightRecoilCharge(G4int charge) {LightRecoilCharge = charge;}
    void SetLightRecoilMass(G4int mass) {LightRecoilMass = mass;}
    void SetHeavyRecoilCharge(G4int charge) {HeavyRecoilCharge = charge;}
    void SetHeavyRecoilMass(G4int mass) {HeavyRecoilMass = mass;}

  private:
    GenericHitsCollection* GetHitsCollection(G4int, const G4Event*) const;
    G4int s1HCID = -1;
    G4int s2HCID = -1;
    G4int icHCID = -1;

    // Define Values here.
    G4double Energy;
    G4double CMEnergy;
    G4double VertexZ;
    G4double QValue;
    G4double ExcitedEnergy;
    G4double LightAngleCM;
    G4double LightAngleLab;
    G4double LightEnergy;
    G4double HeavyAngleCM;
    G4double HeavyAngleLab;
    G4double HeavyEnergy;
    G4int LightRecoilCharge;
    G4int LightRecoilMass;
    G4int HeavyRecoilCharge;
    G4int HeavyRecoilMass;
};


//===================================================================
// Headers for Stepping Action Class
//
#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction*);
    ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
    
  private:
    EventAction* pEventAction;
};


//===================================================================
// Headers for Tracking Action Class
//
#include "G4String.hh"
#include "G4UserTrackingAction.hh"
#include "G4VProcess.hh"

#include "BinaryReactionProcess.hh"
#include "TrackingInformation.hh"

class TrackingAction : public G4UserTrackingAction {
public:
    TrackingAction(EventAction*);
    ~TrackingAction();

    void PreUserTrackingAction(const G4Track*);

private:
    G4String Name;
    EventAction* pEventAction;
};



#endif

    
