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

#include "GenericSD.hh"
#include "G4SDManager.hh"

//===================================================================
// GenericSD class.
//
GenericSD::GenericSD(G4String n) :
  G4VSensitiveDetector(n), 
  hitCollection(nullptr), 
  hcid(-1) {
    collectionName.insert("hitCollection");
}

GenericSD::~GenericSD() {}


void GenericSD::Initialize(G4HCofThisEvent* hce) {

  hitCollection = new GenericHitsCollection(SensitiveDetectorName, collectionName[0]);
  
  if(hcid < 0) {
    hcid = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection);
  }
  hce->AddHitsCollection(hcid, hitCollection);
  hitCollection->insert(new GenericHit());

}


G4bool GenericSD::ProcessHits(G4Step *step, G4TouchableHistory *ROhist) {

  G4double edep = step->GetTotalEnergyDeposit();
  // if(edep / eV < 1) return false;
  // if(step->GetStepLength() < 1.0e-4) return false;
  // G4String type = step->GetTrack()->GetDefinition()->GetParticleType();
  // if(type != "nucleus" && type != "baryon") return false;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();

  //if(step->GetPostStepPoint()) {
    
  G4StepPoint* postStepPoint = step->GetPostStepPoint();
  
  G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyNo = touchable->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();
  G4int trackID = step->GetTrack()->GetTrackID();
  G4ThreeVector position = preStepPoint->GetPosition();
  
  G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();
  //G4cout << "Particle Name: " << particle->GetParticleName() << G4endl;

  G4String target_name = "XX";
  const auto process = step->GetPostStepPoint()->GetProcessDefinedStep();
  if(process->GetProcessName() == "hadElastic") {
    // https://apc.u-paris.fr/~franco/g4doxy/html/classG4HadronicProcess.html
    // https://github.com/hyperk/Geant4/blob/main/source/4.10.01.p02/examples/extended/hadronic/Hadr03/src/SteppingAction.cc
    G4HadronicProcess* hadronicProcess = (G4HadronicProcess*) process;
    const G4Isotope* target = hadronicProcess->GetTargetIsotope();

    //G4cout << "Target Scattering: " << target->GetName() << " Edep: " << edep << G4endl;
    target_name = target->GetName();
  } 

  GenericHit* pHit = new GenericHit();
  pHit->SetHit(copyNo, trackID, hitTime/ns, edep/MeV, position/mm, particle, target_name);
  hitCollection->insert(pHit);
  
  //}
  return true;
}

void GenericSD::EndOfEvent(G4HCofThisEvent*) {}



//===================================================================
// GenericHit class.
//

// G4ThreadLocal G4Allocator<GenericHit>* GenericHitAllocator;

GenericHit::GenericHit() : G4VHit() {}

// GenericHit::GenericHit(
//   G4int id, 
//   G4int trackID, 
//   G4double time, 
//   G4double energy, 
//   G4ThreeVector pos, 
//   G4ParticleDefinition* particle) : 
//     G4VHit(), 
//     ID(id), 
//     TrackID(trackID), 
//     Time(time), 
//     Energy(energy), 
//     Position(pos), 
//     Particle(particle) {}

GenericHit::~GenericHit() {}

void GenericHit::Print() {
  
    // G4cout << "ID, TrackID, Time, Energy, Position.Z, Particle" << G4endl;

    G4cout << ID << ", " 
    << TrackID  << ", "
    << Time  << ", "
    << Energy  << ", "
    << Position.z()  << ", "
    << Particle->GetParticleName()
    << G4endl;

}
