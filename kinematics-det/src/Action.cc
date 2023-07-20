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
  // analysisManager->SetNtupleDirectoryName("tuples");

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
  analysisManager->CreateNtupleDColumn("s1_theta"); 
  analysisManager->CreateNtupleDColumn("s1_phi"); 

  analysisManager->CreateNtupleDColumn("s2_e");
  analysisManager->CreateNtupleDColumn("s2_x"); 
  analysisManager->CreateNtupleDColumn("s2_y"); 
  analysisManager->CreateNtupleDColumn("s2_theta"); 
  analysisManager->CreateNtupleDColumn("s2_phi"); 

  analysisManager->CreateNtupleDColumn("ic_ex");
  analysisManager->CreateNtupleDColumn("ic_ey");
  analysisManager->CreateNtupleDColumn("ic_de");
  analysisManager->CreateNtupleDColumn("ic_e");
  analysisManager->CreateNtupleDColumn("ic_etot");
  analysisManager->CreateNtupleDColumn("ic_x"); 
  analysisManager->CreateNtupleDColumn("ic_y"); 
  analysisManager->CreateNtupleDColumn("ic_theta"); 
  analysisManager->CreateNtupleDColumn("ic_phi"); 
  analysisManager->CreateNtupleDColumn("ic_atomic_mass"); 
  analysisManager->CreateNtupleDColumn("ic_atomic_num"); 

  analysisManager->CreateNtupleDColumn("cm_angle"); 
  
  analysisManager->CreateNtupleDColumn("ThetaCM");         
  analysisManager->CreateNtupleDColumn("ThetaEjectile");   
  analysisManager->CreateNtupleDColumn("ThetaFragment");   
  analysisManager->CreateNtupleDColumn("ThetaDecayLight"); 
  analysisManager->CreateNtupleDColumn("ThetaDecayHeavy"); 
  analysisManager->CreateNtupleDColumn("EjectileKE");      
  analysisManager->CreateNtupleDColumn("FragmentKE");      
  analysisManager->CreateNtupleDColumn("DecayLightKE");    
  analysisManager->CreateNtupleDColumn("DecayHeavyKE");    
  analysisManager->CreateNtupleDColumn("QValue");          
  analysisManager->CreateNtupleDColumn("FragmentEx");      
 
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
EventAction::EventAction() : G4UserEventAction() {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {}

void EventAction::EndOfEventAction(const G4Event* event) {

  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if(!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n";
    // G4Exception("MMEventAction::EndOfEventAction()", "Code001", JustWarning, msg);
    return;
  }

  // Retrieve S1 Related Data
  // 
  // Get hits collections IDs (only once)
  if( s1HCID == -1 ) {
    s1HCID = G4SDManager::GetSDMpointer()->GetCollectionID("S1/hitCollection");
  }
  auto s1HC = GetHitsCollection(s1HCID, event);
  G4double s1Etot = 0.0, s1X = 0.0, s1Y = 0.0, s1Theta = 0.0, s1Phi = 0.0;

  if(s1HC) {
    for(G4int i = 1; i < s1HC->entries(); ++i) {
      s1Etot += (*s1HC)[i]->GetEnergy() / MeV;
      s1X = (*s1HC)[i]->GetPosition().x() / mm;
      s1Y = (*s1HC)[i]->GetPosition().y() / mm;
      s1Theta = (*s1HC)[i]->GetPosition().theta() / deg;
      s1Phi = (*s1HC)[i]->GetPosition().phi() / deg;
    }
  }

  // Retrieve S2 Related Data
  // 
  if( s2HCID == -1 ) {
    s2HCID = G4SDManager::GetSDMpointer()->GetCollectionID("S2/hitCollection");
  }
  auto s2HC = GetHitsCollection(s2HCID, event);
  G4double s2Etot = 0.0, s2X = 0.0, s2Y = 0.0, s2Theta = 0.0, s2Phi = 0.0;

  if(s2HC) {
    for(G4int i = 1; i < s2HC->entries(); ++i) {
      s2Etot += (*s2HC)[i]->GetEnergy() / MeV;
      s2X = (*s2HC)[i]->GetPosition().x() / mm;
      s2Y = (*s2HC)[i]->GetPosition().y() / mm;
      s2Theta = (*s2HC)[i]->GetPosition().theta() / deg;
      s2Phi = (*s2HC)[i]->GetPosition().phi() / deg;
    }
  }

  // Retrieve ic Related Data
  // 
  if( icHCID == -1 ) {
    icHCID = G4SDManager::GetSDMpointer()->GetCollectionID("IC/hitCollection");
  }
  auto icHC = GetHitsCollection(icHCID, event);
  G4double icX = 0.0, icY = 0.0, icTheta = 0.0, icPhi = 0.0;
  G4double icEx = 0.0, icEy = 0.0, icdE = 0.0, icE = 0.0, icEtot = 0.0;
  G4int icAtomicNum = 0.0, icAtomicMass = 0.0;
  
  if(icHC) {
    
    if(icHC->entries() > 1) {
      icAtomicNum = (*icHC)[1]->GetParticle()->GetAtomicNumber();
      icAtomicMass = (*icHC)[1]->GetParticle()->GetAtomicMass();
    }

    for(G4int i = 1; i < icHC->entries(); ++i) {
      auto icHit = (*icHC)[i];
      
      if(icHit->GetPosition().z() / cm > 33.6 && icHit->GetPosition().z() / cm <= 37.6) { // check if the hit was in the "X-region"
        icX = icHit->GetPosition().x() / mm;
        icTheta = icHit->GetPosition().theta() / deg;
        icPhi = icHit->GetPosition().phi() / deg;
        icEx += icHit->GetEnergy() / MeV;
      }
      if(icHit->GetPosition().z() / cm > 37.6 && icHit->GetPosition().z() / cm <= 41.6) { // check if the hit was in the "Y-region"
        icY = icHit->GetPosition().y() / mm;
        icEy += icHit->GetEnergy() / MeV;
      }
      if(icHit->GetPosition().z() / cm > 41.6 && icHit->GetPosition().z() / cm <= 49.6) { // check if the hit was in the "Y-region"
        icY = icHit->GetPosition().y() / mm;
        icdE += icHit->GetEnergy() / MeV;
      }
      if(icHit->GetPosition().z() / cm > 49.6 && icHit->GetPosition().z() / cm <= 69.6) { // check if the hit was in the "Y-region"
        icE += icHit->GetEnergy() / MeV;
      }
    
      icEtot += icHit->GetEnergy() / MeV;
    }
  }

  auto analysisManager = G4AnalysisManager::Instance();
  
  // Fill S1 Related Data
  // 
  analysisManager->FillNtupleDColumn(0, 0, s1Etot);
  analysisManager->FillNtupleDColumn(0, 1, s1X); 
  analysisManager->FillNtupleDColumn(0, 2, s1Y);
  analysisManager->FillNtupleDColumn(0, 3, s1Theta);
  analysisManager->FillNtupleDColumn(0, 4, s1Phi);

  // Fill S2 Related Data
  // 
  analysisManager->FillNtupleDColumn(0, 5, s2Etot); 
  analysisManager->FillNtupleDColumn(0, 6, s2X); 
  analysisManager->FillNtupleDColumn(0, 7, s2Y); 
  analysisManager->FillNtupleDColumn(0, 8, s2Theta);
  analysisManager->FillNtupleDColumn(0, 9, s2Phi);

  // Fill ic Related Data
  // 
  analysisManager->FillNtupleDColumn(0, 10, icEx); 
  analysisManager->FillNtupleDColumn(0, 11, icEy); 
  analysisManager->FillNtupleDColumn(0, 12, icdE); 
  analysisManager->FillNtupleDColumn(0, 13, icE); 
  analysisManager->FillNtupleDColumn(0, 14, icEtot); 
  analysisManager->FillNtupleDColumn(0, 15, icX); 
  analysisManager->FillNtupleDColumn(0, 16, icY); 
  analysisManager->FillNtupleDColumn(0, 17, icTheta);
  analysisManager->FillNtupleDColumn(0, 18, icPhi);
  analysisManager->FillNtupleDColumn(0, 19, icAtomicMass);
  analysisManager->FillNtupleDColumn(0, 20, icAtomicNum);
 
  // Fill rxn stuff 
  analysisManager->FillNtupleDColumn(0, 21, ThetaCM);         
  analysisManager->FillNtupleDColumn(0, 22, ThetaEjectile);   
  analysisManager->FillNtupleDColumn(0, 23, ThetaFragment);   
  analysisManager->FillNtupleDColumn(0, 24, ThetaDecayLight); 
  analysisManager->FillNtupleDColumn(0, 25, ThetaDecayHeavy); 
  analysisManager->FillNtupleDColumn(0, 26, EjectileKE);      
  analysisManager->FillNtupleDColumn(0, 27, FragmentKE);      
  analysisManager->FillNtupleDColumn(0, 28, DecayLightKE);    
  analysisManager->FillNtupleDColumn(0, 29, DecayHeavyKE);    
  analysisManager->FillNtupleDColumn(0, 30, QValue);          
  analysisManager->FillNtupleDColumn(0, 31, FragmentEx);      
 

  analysisManager->AddNtupleRow();
}


GenericHitsCollection* EventAction::GetHitsCollection(G4int hcid, const G4Event* event) const {
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
void SteppingAction::UserSteppingAction(const G4Step *step) {}


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
  pEventAction->SetThetaCM(info->GetThetaCM()); 
  pEventAction->SetThetaEjectile(info->GetThetaEjectile());
  pEventAction->SetThetaFragment(info->GetThetaFragment());
  pEventAction->SetThetaDecayLight(info->GetThetaDecayLight()); 
  pEventAction->SetThetaDecayHeavy(info->GetThetaDecayHeavy());
  pEventAction->SetEjectileKE(info->GetEjectileKE()); 
  pEventAction->SetFragmentKE(info->GetFragmentKE());
  pEventAction->SetDecayLightKE(info->GetDecayLightKE());
  pEventAction->SetDecayHeavyKE(info->GetDecayHeavyKE());
  pEventAction->SetQValue(info->GetQValue());
  pEventAction->SetFragmentEx(info->GetFragmentEx());

  // pEventAction->SetHeavyRecoilCharge(info->GetHeavyRecoil()->GetAtomicNumber());
  // pEventAction->SetHeavyRecoilMass(info->GetHeavyRecoil()->GetAtomicMass());

}
