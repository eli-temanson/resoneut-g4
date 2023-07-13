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
ActionInitialization::ActionInitialization() 
  : G4VUserActionInitialization() 
{ 

}
ActionInitialization::~ActionInitialization() { }

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
}


//===================================================================
// Run Action Class.
//
RunAction::RunAction() 
{ 
  // Get analysis manager
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetFileName("data/ouput");
  analysisManager->SetNtupleDirectoryName("tuples");

  // Creating histograms
  //
  // analysisManager->CreateH1("Edep", "Edep", 300, 0.0, 600 * keV); // h2 Id = 0

  // Creating ntuple
  //
  analysisManager->CreateNtuple("hits", "hits");
  analysisManager->CreateNtupleIColumn("fEvent");
  analysisManager->CreateNtupleDColumn("fX");
  analysisManager->CreateNtupleDColumn("fY");
  analysisManager->CreateNtupleDColumn("fZ");
  analysisManager->CreateNtupleDColumn("fT");
  analysisManager->FinishNtuple();
  // analysisManager->SetNtupleFileName(0, "data/Ntuple-hits");

  analysisManager->CreateNtuple("events", "events");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->FinishNtuple();
  // analysisManager->SetNtupleFileName(1, "data/Ntuple-events");

}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
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

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
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
EventAction::EventAction() {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) { }

void EventAction::EndOfEventAction(const G4Event* event) 
{
  // Get hits collections IDs (only once)
  if ( scintHCID == -1 ) 
  {
    scintHCID = G4SDManager::GetSDMpointer()->GetCollectionID("ScintHitsCollection");
  }

  // Get hits collections
  auto scintHC = GetHitsCollection(scintHCID, event);
  // Get hit with total values
  auto scintHit = (*scintHC)[0];
  // G4cout << "----- Hit Entries: "<< scintHC->entries() << G4endl;

  // G4cout << "----------- "<< scintHit->GetEdep() << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(1, 0, scintHit->GetEdep()/keV); 
  analysisManager->AddNtupleRow(1);
}


ScintHitsCollection* EventAction::GetHitsCollection(G4int hcid,const G4Event* event) const
{
  auto hitsCollection = static_cast<ScintHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcid));

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
SteppingAction::SteppingAction(EventAction* event) : fEventAction(event) {}
SteppingAction::~SteppingAction(){}

void SteppingAction::UserSteppingAction(const G4Step *step) 
{
  // fEventAction->fEdep += step->GetTotalEnergyDeposit();

  // if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Scintillation")
  // {
  //   G4cerr << "Process: " << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
  // }
  
  //static G4ParticleDefinition* opticalphoton = G4OpticalPhoton::OpticalPhotonDefinition();
}
