
#include "TrackingInformation.hh"

TrackingInformation::TrackingInformation(
    G4double theta_cm, 
    G4double theta_ejectile,
    G4double theta_fragment, 
    G4double theta_decay_light, 
    G4double theta_deacy_heavy,
    G4double ke_ejectile, 
    G4double ke_fragment, 
    G4double ke_decay_light,
    G4double ke_decay_heavy,
    G4double qValue, 
    G4double frag_ex)
{
    ThetaCM = theta_cm; 
    ThetaEjectile = theta_ejectile;
    ThetaFragment = theta_fragment;
    ThetaDecayLight = theta_decay_light; 
    ThetaDecayHeavy = theta_deacy_heavy;
    EjectileKE = ke_ejectile; 
    FragmentKE = ke_fragment;
    DecayLightKE = ke_decay_light;
    DecayHeavyKE = ke_decay_heavy;
    QValue = qValue;
    FragmentEx = frag_ex;
}


TrackingInformation::~TrackingInformation() {}

void TrackingInformation::Print() const {}
