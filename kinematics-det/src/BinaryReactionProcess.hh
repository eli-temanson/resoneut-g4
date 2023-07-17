#ifndef BinaryReactionProcess_h
#define BinaryReactionProcess_h

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VDiscreteProcess.hh"

#include "DetectorConstruction.hh"
#include "TrackingInformation.hh"

#include <cmath>

class BinaryReactionProcess : public G4VDiscreteProcess {
public:
  BinaryReactionProcess(const G4String& name = "BinaryReaction");
  ~BinaryReactionProcess();

  G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*);
  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

  G4VParticleChange* Decay(const G4Track&, G4int, G4int, G4int, G4int);

  void StartTracking(G4Track*);


private:
  G4double fScatteringEnergy;
};

#endif
