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
  hcid(-1) 
  {
  // hcid = -1;
  collectionName.insert("hitCollection");
}

GenericSD::~GenericSD() {}

// void GenericSD::Initialize(G4HCofThisEvent* hce) {
//   hitCollection = new GenericHitsCollection(GetName(), collectionName[0]);
//   static G4int hcid = GetCollectionID(0);  
//   hce->AddHitsCollection(hcid, hitCollection);
//   hitCollection->insert(new GenericHit());
// }

void GenericSD::Initialize(G4HCofThisEvent* hce) {

  hitCollection = new GenericHitsCollection(SensitiveDetectorName, collectionName[0]);
  
  if(hcid < 0) {
    hcid = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection);
  }
  hce->AddHitsCollection(hcid, hitCollection);
  hitCollection->insert(new GenericHit());

  // static G4int hcid = GetCollectionID(0);  
  // hce->AddHitsCollection(hcid, hitCollection);
  // hitCollection->insert(new GenericHit());
  // hce->AddHitsCollection(HCID, hitCollection);

}


G4bool GenericSD::ProcessHits(G4Step *step, G4TouchableHistory *ROhist) {

  // auto hit = (*hitCollection)[0];
  // if (step->GetPreStepPoint()->GetProcessDefinedStep()) {
  //   // G4cout << "ProcName: " << step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
  //   // if (step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation") return true;
  //   // if (step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() == "initStep") return true;
  // } else {
  //   return true;
  // }

  G4double edep = step->GetTotalEnergyDeposit();
  // if(edep / eV < 1) return true;

  // G4cout << SensitiveDetectorName <<" Hit edep: " << edep << G4endl;

  G4String type = step->GetTrack()->GetDefinition()->GetParticleType();
  // if(type != "nucleus" && type != "baryon") return true;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  // if(step->GetPostStepPoint()) { 
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4int copyNo = touchable->GetVolume()->GetCopyNo();
    G4double hitTime = preStepPoint->GetGlobalTime();
    G4int trackID = step->GetTrack()->GetTrackID();
    G4ThreeVector position = preStepPoint->GetPosition();
    G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();

    GenericHit* pHit = new GenericHit();
    pHit->SetHit(copyNo, trackID, hitTime/ns, edep/MeV, position/mm, particle);
    hitCollection->insert(pHit);
  // }
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
