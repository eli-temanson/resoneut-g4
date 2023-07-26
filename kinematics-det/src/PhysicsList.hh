

#ifndef PhysicsList_hh
#define PhysicsList_hh 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VModularPhysicsList {
  public:
    PhysicsList();
    ~PhysicsList();

  public:
    void SetCuts() override;
    // virtual void SetCuts();

  private:

};

#endif
