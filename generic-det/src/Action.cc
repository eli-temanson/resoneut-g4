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

#include "Action.hh"

#include "G4OpticalPhoton.hh"

//===================================================================
// Action Initalizaiton Class.
//
ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

// Add all Actions
// 
void ActionInitialization::BuildForMaster() const {
  SetUserAction(new RunAction);
}
void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction); 

  auto eventAction = new EventAction;
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction(eventAction));
  SetUserAction(new TrackingAction(eventAction));

}


//===================================================================
// Run Action Class.
//
RunAction::RunAction() { 
  // Get analysis manager
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetFileName("analysis/ouput");
  analysisManager->SetNtupleDirectoryName("tuples");

  // Creating histograms
  //
  // analysisManager->CreateH1("Edep", "Edep", 300, 0.0, 600 * keV); // h2 Id = 0

  // Creating ntuple
  //
  // analysisManager->CreateNtuple("hits", "hits");
  // analysisManager->CreateNtupleIColumn("fEvent");
  // analysisManager->CreateNtupleDColumn("fX");
  // analysisManager->CreateNtupleDColumn("fY");
  // analysisManager->CreateNtupleDColumn("fZ");
  // analysisManager->CreateNtupleDColumn("fT");
  // analysisManager->FinishNtuple();
  // analysisManager->SetNtupleFileName(0, "data/Ntuple-hits");

  analysisManager->CreateNtuple("events", "events");
  analysisManager->CreateNtupleDColumn("edep_SD");
  analysisManager->CreateNtupleDColumn("edep_event");
  analysisManager->FinishNtuple();
  // analysisManager->SetNtupleFileName(1, "data/Ntuple-events");

}

RunAction::~RunAction() {
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*) {
  // Open an output file
  //
  auto analysisManager = G4AnalysisManager::Instance();

  //Reset histograms from previous run
  //
  analysisManager->Reset(); 

  // Open an output file, The default file name is set in RunAction::RunAction(), it can be overwritten in a macro
  //
  analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* /*run*/) {
  // save histograms & ntuple
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  // analysisManager->CloseFile(false);
}
//===================================================================


//===================================================================
// Event Action Class
//
EventAction::EventAction() : G4UserEventAction() {
  s1HCID = -1;
}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
  // G4SDManager* pSDmanager = G4SDManager::GetSDMpointer();
  // s1HCID = G4SDManager::GetSDMpointer()->GetCollectionID("S1/genCollection");

  Energy = 0.0;

}

void EventAction::EndOfEventAction(const G4Event* event) {
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if(!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n";
    // G4Exception("MMEventAction::EndOfEventAction()", "Code001", JustWarning, msg);
    return;
  }

  // Get hits collections IDs (only once)
  if( s1HCID == -1 ) {
    s1HCID = G4SDManager::GetSDMpointer()->GetCollectionID("S1/genCollection");
    G4cout << "s1HCID Set: " << s1HCID << G4endl;
  }

  GenericHitsCollection* S1;
  S1 = static_cast<GenericHitsCollection*>(hce->GetHC(s1HCID));

  // Get hits collections
  auto s1HC = GetHitsCollection(s1HCID, event);
  // Get hit with total values
  auto s1Hit = (*s1HC)[0];
  G4cout << "debug-1" << G4endl;

  // for(G4int i = 0; i < (s1HC->entries()); ++i) {
    G4cout << (*s1HC)[0]->GetEdep() / keV << G4endl;
    G4cout << Energy / keV << G4endl;
  // }


  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, 0, s1Hit->GetEdep() / keV); 
  analysisManager->FillNtupleDColumn(0, 1, Energy / keV);

  analysisManager->AddNtupleRow();
}


GenericHitsCollection* EventAction::GetHitsCollection(G4int hcid, const G4Event* event) const
{
  auto hitsCollection = static_cast<GenericHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcid));

  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcid;
    G4Exception("EventAction::GetHitsCollection()","MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}



//===================================================================
// Stepping Action Class.
//
SteppingAction::SteppingAction(EventAction* event) : pEventAction(event) {}
SteppingAction::~SteppingAction(){}

void SteppingAction::UserSteppingAction(const G4Step *step) {

  pEventAction->AddEnergy(step->GetTotalEnergyDeposit());

}


//===================================================================
// Tracking Action Class.
//
TrackingAction::TrackingAction(EventAction* event) : pEventAction(event) {
  Name = "BinaryReaction";
}
TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if(!creatorProcess) return;
  // if(creatorProcess->GetProcessName() != Name) return;
  // if(track->GetTrackID() != 2) return;

  TrackingInformation* info = (TrackingInformation*) track->GetUserInformation();

  // These need to be defined in the event action class (Action.hh)
  //
  pEventAction->AddEnergy(info->GetEnergy());
  // fEventAction->SetCMEnergy(info->GetCMEnergy());
  // fEventAction->SetVertexZ(info->GetVertex().z());
  // fEventAction->SetQValue(info->GetQValue());
  // fEventAction->SetExcitedEnergy(info->GetExcitedEnergy());

  // fEventAction->SetLightAngleCM(info->GetCMLightTheta());
  // fEventAction->SetLightAngleLab(info->GetLabLightTheta());
  // fEventAction->SetLightEnergy(info->GetLightEnergy());
  // fEventAction->SetLightRecoilCharge(info->GetLightRecoil()->GetAtomicNumber());
  // fEventAction->SetLightRecoilMass(info->GetLightRecoil()->GetAtomicMass());

  // fEventAction->SetHeavyAngleCM(info->GetCMHeavyTheta());
  // fEventAction->SetHeavyAngleLab(info->GetLabHeavyTheta());
  // fEventAction->SetHeavyEnergy(info->GetHeavyEnergy());
  // fEventAction->SetHeavyRecoilCharge(info->GetHeavyRecoil()->GetAtomicNumber());
  // fEventAction->SetHeavyRecoilMass(info->GetHeavyRecoil()->GetAtomicMass());

}
