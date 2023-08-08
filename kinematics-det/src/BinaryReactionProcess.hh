
#ifndef BinaryReactionProcess_h
#define BinaryReactionProcess_h

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VDiscreteProcess.hh"
#include "Randomize.hh"

#include "TGraph.h"
#include "TF1.h"

#include "DetectorConstruction.hh"
#include "TrackingInformation.hh"
#include "TypeDef.hh"
#include "spline.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>

class BinaryReactionProcess : public G4VDiscreteProcess {
public:
  BinaryReactionProcess(const G4String& name = "BinaryReaction");
  ~BinaryReactionProcess();

  G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*);
  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

  G4VParticleChange* Decay(const G4Track&, G4int, G4int, G4int, G4int);

  void StartTracking(G4Track*);
  static double MyAngDis(double*,double*);
private:
  G4double fScatteringEnergy;
  kinematics_struct Beam, Target, Ejectile, Fragment, Decay_Heavy, Decay_Light;
  std::vector<double> X;
  std::vector<double> Y; 
  tk::spline angDis;
  // TRandom3 *Rndm = new TRandom3();
  TGraph *angdis;
  
  void SetAngDis(const std::string &filename){
    std::ifstream input(filename.c_str()); 
    if(!input.is_open()){
      std::cout<<"Could not Load Angular Distribution from: "<< filename << "\n";
      exit(0);
    } else{
      std::cout<<"Loaded Angular Distribution from: "<< filename << "\n";
    }
    double tempX = 0.0, tempY = 0.0, max_value = 0.0;
    double sumY = 0.0;
    while(input >> tempX >> tempY){
      if(tempY > max_value) { 
        max_value = tempY; 
      }
      X.push_back(tempX);
      Y.push_back(tempY);
      sumY = sumY + tempY;
    }
    std::transform(Y.begin(), Y.end(), Y.begin(), [&max_value](auto& c) { return c / max_value;});
    //angDis = tk::spline(X,Y);
    angdis = new TGraph(X.size(), &X[0],&Y[0]);
    input.close();
  }


  double GetAngDis(double x){
    return angdis->Eval(x);
  }
  
  double GetInvKinTheta(){
    double thetaCM = 0.0;
    do{ // from dwba, randomize neutron events going from regular to inverse kin (180-theta)
      // create a uniform distribution in radians 
      thetaCM = std::acos(1.0 - (2.0*G4UniformRand())); 
    // Weight that distribution by the Normalized Ang Dis. 
    //} while (G4UniformRand() > angDis(180.0 - (thetaCM / degree))); 
    } while (G4UniformRand() > GetAngDis(180.0 - (thetaCM / degree))); 
    //} while (( 180.0 - thetaCM / degree) > 55.0 || G4UniformRand() > GetAngDis(180.0 - (thetaCM / degree))); 
    
    return thetaCM;
    //return angdis_f1->GetRandom(); 
  }


};

#endif
