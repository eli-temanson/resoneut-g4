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
#include "PhysicsList.hh"
#include "NucleonStates.hh"

// Run Manager
#include "G4RunManagerFactory.hh"

// Gui Support 
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// Main physics
#include "G4VModularPhysicsList.hh"
// #include "FTFP_BERT_HP.hh"

// Allows user to choose the random engine
#include "Randomize.hh"

#include "InputReader.hh"
#include <iostream>
#include <string>

int main(int argc,char** argv) {

  if(argc<2) {
    std::cerr<<"Incorrect number of arguments!"<<std::endl;
    std::cout<<"If you need help, add -h or --help for assistance"<<std::endl;
    return 1;
  } else if (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "--help")==0) {
    std::cout<<"Welcome To The RESONEUT Geant4 Simulation!"<<std::endl;
    std::cout<<"To run this code you need to provide a *.json file as an argument"<<std::endl;
    std::cout<<"Example: ./path/to/program assets/input.json"<<std::endl;
    std::cout<<"See the README.md for more details"<<std::endl;
    return 1;
  }

  // Initialize the input reader with the file name.
  // In other classes it can now be called without the filename.
  InputReader* reader = InputReader::Instance(std::string(argv[1]));
  std::cout << reader->GetFileOutput() << std::endl;
  NucleonStates* states = NucleonStates::Instance();

  // Construct the default run manager
  //
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Detector construction
  //
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  //
  // runManager->SetUserInitialization(new FTFP_BERT_HP());
  runManager->SetUserInitialization(new PhysicsList());

  // User action initialization
  //
  runManager->SetUserInitialization(new ActionInitialization());

  // //choose the Random engine
  // CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  // //set random seed with system time
  // G4long seed = time(NULL);
  // // if(argc>2) seed += 473879*atoi(argv[2]);
  // CLHEP::HepRandom::setTheSeed(seed);

  //===================================================================
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
    // G4String fileName = argv[1];
    // UImanager->ApplyCommand(command+fileName);
    UImanager->ApplyCommand(command+reader->GetFileMacro());
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

