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
  analysisManager->CreateNtupleDColumn("s1_e");
  analysisManager->CreateNtupleDColumn("s1_x"); 
  analysisManager->CreateNtupleDColumn("s1_y"); 

  analysisManager->CreateNtupleDColumn("s2_e");
  analysisManager->CreateNtupleDColumn("s2_x"); 
  analysisManager->CreateNtupleDColumn("s2_y"); 

  // analysisManager->CreateNtupleDColumn("edep_event");
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
}
//===================================================================


//===================================================================
// Event Action Class
//
EventAction::EventAction() : G4UserEventAction() {
  // s1HCID = -1;
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
    s1HCID = G4SDManager::GetSDMpointer()->GetCollectionID("S1/hitCollection");
    // G4cout << "s1HCID: " << s1HCID << G4endl;
  }
  // pHitCol = static_cast<GenericHitsCollection*>(hce->GetHC(s1HCID));
  auto s1HC = GetHitsCollection(s1HCID, event);
  // auto s1Hit = (*s1HC)[0]; // Get hit with total values
  G4double s1Etot = 0.0;
  G4double s1X = 0.0;
  G4double s1Y = 0.0;
  if(s1HC) {
    for(G4int i = 1; i < s1HC->entries(); ++i) {
      s1Etot += (*s1HC)[i]->GetEnergy() / MeV;
    }
    s1X = (*s1HC)[0]->GetPosition().x();
    s1Y = (*s1HC)[0]->GetPosition().y();
  }
  // G4cout << "s1Etot: " << s1Etot << G4endl;


  if( s2HCID == -1 ) {
    s2HCID = G4SDManager::GetSDMpointer()->GetCollectionID("S2/hitCollection");
    // G4cout << "s2HCID: " << s2HCID << G4endl;
  }
  // GenericHitsCollection* s2HC;
  // s2HC = static_cast<GenericHitsCollection*>(hce->GetHC(s2HCID));
  auto s2HC = GetHitsCollection(s2HCID, event);
  G4double s2Etot = 0.0;
  G4double s2X = 0.0;
  G4double s2Y = 0.0;
  if(s2HC) {
    for(G4int i = 1; i < s2HC->entries(); ++i) {
      s2Etot += (*s2HC)[i]->GetEnergy() / MeV;
    }
    s2X = (*s2HC)[0]->GetPosition().x();
    s2Y = (*s2HC)[0]->GetPosition().y();
  }
  // G4cout << "s2Etot: " << s2Etot << G4endl;
  // G4cout << "Etot: " << s1Etot+s2Etot << G4endl;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, 0, s1Etot);
  analysisManager->FillNtupleDColumn(0, 1, s1X); 
  analysisManager->FillNtupleDColumn(0, 2, s1Y); 

  analysisManager->FillNtupleDColumn(0, 3, s2Etot); 
  analysisManager->FillNtupleDColumn(0, 4, s2X); 
  analysisManager->FillNtupleDColumn(0, 5, s2Y); 

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

  // G4cout << "Stepping Action" << G4endl;
  // G4cout << "Step edep: " << step->GetTotalEnergyDeposit() << G4endl;
  // pEventAction->AddEnergy(step->GetTotalEnergyDeposit());

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
  if(creatorProcess->GetProcessName() != Name) return;
  if(track->GetTrackID() != 2) return;

  TrackingInformation* info = (TrackingInformation*) track->GetUserInformation();

  // These need to be defined in the event action class (Action.hh)
  //
  pEventAction->AddEnergy(info->GetEnergy());
  pEventAction->SetCMEnergy(info->GetCMEnergy());
  pEventAction->SetVertexZ(info->GetVertex().z());
  pEventAction->SetQValue(info->GetQValue());
  pEventAction->SetExcitedEnergy(info->GetExcitedEnergy());
  pEventAction->SetLightAngleCM(info->GetCMLightTheta());
  pEventAction->SetLightAngleLab(info->GetLabLightTheta());
  pEventAction->SetLightEnergy(info->GetLightEnergy());
  pEventAction->SetLightRecoilCharge(info->GetLightRecoil()->GetAtomicNumber());
  pEventAction->SetLightRecoilMass(info->GetLightRecoil()->GetAtomicMass());
  pEventAction->SetHeavyAngleCM(info->GetCMHeavyTheta());
  pEventAction->SetHeavyAngleLab(info->GetLabHeavyTheta());
  pEventAction->SetHeavyEnergy(info->GetHeavyEnergy());
  pEventAction->SetHeavyRecoilCharge(info->GetHeavyRecoil()->GetAtomicNumber());
  pEventAction->SetHeavyRecoilMass(info->GetHeavyRecoil()->GetAtomicMass());

}