

#ifndef PhysicsList_hh
#define PhysicsList_hh 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class PhysicsListMessenger;
class G4PhysListFactoryMessenger;

class PhysicsList: public G4VModularPhysicsList {
  public:
    PhysicsList();
    ~PhysicsList();

  public:
    // void SetCuts() override;
    virtual void SetCuts();
    virtual void ConstructParticle();
    virtual void ConstructProcess();    

  private:
    G4VPhysicsConstructor*  pEmPhysicsList;
    G4VPhysicsConstructor*  pParticleList;
    G4VPhysicsConstructor*  pHadronPhys;

};

#endif
