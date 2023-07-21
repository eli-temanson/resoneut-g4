
#ifndef TrackingInformation_h
#define TrackingInformation_h

#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4VUserTrackInformation.hh"

class TrackingInformation : public G4VUserTrackInformation {

public:
  TrackingInformation(
    G4double, 
    G4double, 
    G4double, 
    G4double, 
    G4double,
    G4double, 
    G4double, 
    G4double, 
    G4double, 
    G4double, 
    G4double);
  ~TrackingInformation();

  void Print() const;

  G4double GetThetaCM() const {return ThetaCM;} 
  G4double GetThetaEjectile() const {return ThetaEjectile;}       
  G4double GetThetaFragment() const {return ThetaFragment;}      
  G4double GetThetaDecayLight() const {return ThetaDecayLight;}  
  G4double GetThetaDecayHeavy() const {return ThetaDecayHeavy;} 
  G4double GetEjectileKE() const {return EjectileKE;} 
  G4double GetFragmentKE() const {return FragmentKE;}            
  G4double GetDecayLightKE() const {return DecayLightKE;}
  G4double GetDecayHeavyKE() const {return DecayHeavyKE;}       
  G4double GetQValue() const {return QValue;}       
  G4double GetFragmentEx() const {return FragmentEx;}                 

  void SetThetaCM(G4double theta_cm) {ThetaCM = theta_cm;} 
  void SetThetaEjectile(G4double theta_ejectile) {ThetaEjectile = theta_ejectile;}       
  void SetThetaFragment(G4double theta_fragment) {ThetaFragment = theta_fragment;}      
  void SetThetaDecayLight(G4double theta_decay_light) {ThetaDecayLight = theta_decay_light;}  
  void SetThetaDecayHeavy(G4double theta_deacy_heavy) {ThetaDecayHeavy = theta_deacy_heavy;} 
  void SetEjectileKE(G4double ke_ejectile) {EjectileKE = ke_ejectile;} 
  void SetFragmentKE(G4double ke_fragment) {FragmentKE = ke_fragment;}            
  void SetDecayLightKE(G4double ke_decay_light) {DecayLightKE = ke_decay_light;}
  void SetDecayHeavyKE(G4double ke_decay_heavy) {DecayHeavyKE = ke_decay_heavy;}       
  void SetQValue(G4double qValue) {QValue = qValue;}       
  void SetFragmentEx(G4double frag_ex) {FragmentEx = frag_ex;}                 

private:
  
  G4double ThetaCM; 
  G4double ThetaEjectile;
  G4double ThetaFragment;
  G4double ThetaDecayLight; 
  G4double ThetaDecayHeavy;
  G4double EjectileKE; 
  G4double FragmentKE;
  G4double DecayLightKE;
  G4double DecayHeavyKE;
  G4double QValue;
  G4double FragmentEx;

};

#endif
