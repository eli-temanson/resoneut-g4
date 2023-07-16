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

#include "ScintSD.hh"

ScintSD::ScintSD(G4String name) : 
    G4VSensitiveDetector(name),
    hitCollection(nullptr)
{
    collectionName.insert("ScintHitsCollection");
}

ScintSD::~ScintSD() {}


void ScintSD::Initialize(G4HCofThisEvent* hce)
{
    hitCollection = new ScintHitsCollection(GetName(),collectionName[0]);

    static G4int hcid = GetCollectionID(0);  
    hce->AddHitsCollection(hcid, hitCollection);
    hitCollection->insert(new ScintHit());
}


G4bool ScintSD::ProcessHits(G4Step *step, G4TouchableHistory *ROhist)
{
    auto hit = (*hitCollection)[0];
    if ( ! hit ) 
    {
        G4ExceptionDescription msg;
        msg << "Cannot access hit: " << G4endl;
        G4Exception("ScintSD::ProcessHits()","MyCode0004", FatalException, msg);
    }

    // Add a Gaussian Broadening to the deposited energy
    G4double edep_spread = G4RandGauss::shoot(step->GetTotalEnergyDeposit(), 13*keV);
    // G4double edep_spread = step->GetTotalEnergyDeposit();
    // if(edep_spread > 37*keV) // enfore threshold for each step
    // {
        hit->AddEdep(edep_spread);
    // }

    // G4Track *track = step->GetTrack();
    //track->SetTrackStatus(fStopAndKill);
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    // G4StepPoint *postStepPoint = step->GetPostStepPoint();
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();
    G4double time = preStepPoint->GetGlobalTime();
    // G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
    // G4cout << "Photon position: " << posPhoton << G4endl;
    // const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
    // G4int copyNo = touchable->GetCopyNumber();
    // G4cout << "Copy number: " << copyNo << G4endl;
    // G4VPhysicalVolume *physVol = touchable->GetVolume();
    // G4ThreeVector posDetector = physVol->GetTranslation();

    // #ifndef G4MULTITHREADED
        //G4cout << "Detector position: " << posDetector << G4endl;
        //G4cout << "Photon wavelength: " << wlen << G4endl;
    // #endif

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, 0, evt);
    analysisManager->FillNtupleDColumn(0, 1, posPhoton[0]);
    analysisManager->FillNtupleDColumn(0, 2, posPhoton[1]);
    analysisManager->FillNtupleDColumn(0, 3, posPhoton[2]);
    analysisManager->FillNtupleDColumn(0, 4, time);

    analysisManager->AddNtupleRow(0);

    return true;
}


void ScintSD::EndOfEvent(G4HCofThisEvent*)
{

}

//===================================================================

ScintHit::ScintHit() : 
    G4VHit(), 
    edep(0.0)
{

}

ScintHit::~ScintHit() {}