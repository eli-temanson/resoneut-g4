
// #include "TrackingAction.hh"

// TrackingAction::TrackingAction(EventAction* eventAction) : fEventAction(eventAction) {
//     fName = "BinaryReaction";
// }

// TrackingAction::~TrackingAction() {}

// void TrackingAction::PreUserTrackingAction(const G4Track* track) {
//     const G4VProcess* creatorProcess = track->GetCreatorProcess();
//     if(!creatorProcess) return;

//     if(creatorProcess->GetProcessName() != fName) return;

//     if(track->GetTrackID() != 2) return;

//     // TrackingInformation* info = (TrackingInformation*) track->GetUserInformation();

//     // // These need to be defined in the event action class (Action.hh)
//     //
//     // fEventAction->SetEnergy(info->GetEnergy());
//     // fEventAction->SetCMEnergy(info->GetCMEnergy());
//     // fEventAction->SetVertexZ(info->GetVertex().z());
//     // fEventAction->SetQValue(info->GetQValue());
//     // fEventAction->SetExcitedEnergy(info->GetExcitedEnergy());

//     // fEventAction->SetLightAngleCM(info->GetCMLightTheta());
//     // fEventAction->SetLightAngleLab(info->GetLabLightTheta());
//     // fEventAction->SetLightEnergy(info->GetLightEnergy());
//     // fEventAction->SetLightRecoilCharge(info->GetLightRecoil()->GetAtomicNumber());
//     // fEventAction->SetLightRecoilMass(info->GetLightRecoil()->GetAtomicMass());

//     // fEventAction->SetHeavyAngleCM(info->GetCMHeavyTheta());
//     // fEventAction->SetHeavyAngleLab(info->GetLabHeavyTheta());
//     // fEventAction->SetHeavyEnergy(info->GetHeavyEnergy());
//     // fEventAction->SetHeavyRecoilCharge(info->GetHeavyRecoil()->GetAtomicNumber());
//     // fEventAction->SetHeavyRecoilMass(info->GetHeavyRecoil()->GetAtomicMass());

// }
