#ifndef TrackingAction_h
#define TrackingAction_h

#include "G4String.hh"
#include "G4UserTrackingAction.hh"
#include "G4VProcess.hh"

#include "BinaryReactionProcess.hh"
#include "Action.hh"
#include "TrackingInformation.hh"

class TrackingAction : public G4UserTrackingAction {
public:
    TrackingAction(EventAction*);
    ~TrackingAction();

    void PreUserTrackingAction(const G4Track*);

private:
    G4String fName;
    EventAction* fEventAction;
};

#endif
