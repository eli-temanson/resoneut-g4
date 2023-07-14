//===================================================================
//  Geant4 Simulation of para-Terphenyl scintillator
//
//  This file contains the main function, which includes and calls->
//    > Run Manager
//    > Detector Construction
//    > Physics Lists
//    > Action Initialization
//    > User Interface / Batch Mode (Don't Touch)
//
//  Date of Last Modified: 12/15/2021
//  Author: Eli Temanson
//===================================================================

// User Defined Classes
#include "DetectorConstruction.hh"
#include "Action.hh"
// #include "BinaryReactionPhysics.hh"

// Run Manager
#include "G4RunManagerFactory.hh"

// Physics list
#include "G4VModularPhysicsList.hh"
#include "QGSP_BERT_HP.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
//#include "G4OpticalParameters.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"

// Gui Support 
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// Allows user to choose the random engine
#include "Randomize.hh"

// using json-c++ reader
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main(int argc,char** argv)
{
  // Construct the default run manager
  //
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Detector construction
  //
  runManager->SetUserInitialization(new DetectorConstruction());
  
  // Physics list
  //
  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
  // physicsList->SetVerboseLevel(0);
  // physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  // G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  // physicsList->RegisterPhysics(opticalPhysics);
  // BinaryReactionPhysics* reactionPhysics = new BinaryReactionPhysics();
  // physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  // physicsList->RegisterPhysics(reactionPhysics);

  runManager->SetUserInitialization(physicsList); 

  // User action initialization
  //
  runManager->SetUserInitialization(new ActionInitialization());
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = nullptr; // 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Process macro or start UI session
  //
  if( !ui ) { // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  } else { // interactive mode
    UImanager->ApplyCommand("/control/execute macros/vis.mac");
    ui->SessionStart();
    delete ui;
  }


  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !  
  // #ifdef G4VIS_USE
  //   delete visManager;
  // #endif
  //   // delete runManager;

  // delete visManager;
  // delete runManager;
  return 0;
}

