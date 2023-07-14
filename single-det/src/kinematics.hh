
#ifndef kinematics_h
#define kinematics_h

//C and C++ libraries.
#include <cmath>
#include <iostream>
#include <fstream>

#include "TROOT.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

namespace kinematics {

  struct Particle {
      double M,InvMass,KE,E,Ecm;
      double x, y, z;
      double Theta,Phi;
      double Px,Py,Pz,P;
      TLorentzVector LV;
      TVector3 Pos;
      int A, Z;
  };

  class Kinematics{
  protected:
    // double mass[6];
    double FragEst=0.0;
    double BeamSigma=0.0, MeanBeamEnergy=0.0;
    TRandom3 *Rndm = new TRandom3();
    
  public:
    Kinematics(){};
    ~Kinematics(){};

    Particle Beam,Target,Ejectile,Fragment,Decay_Light,Decay_Heavy;

    void SetReaction(int nucleus[][2]);
    double QValue = 0.0;
    double DecayQValueEst = 0.0;
    double DecayQValue = 0.0;
    double ExcEnergy = 0.0;
    double InvMassExcEnergy = 0.0;
    double RecoilQValue = 0.0;
    double ThetaCM = 0.0;
    double Sp = 0.0;
    double gamma,term1,term2,term3;
    double Ecm,Pcm;

    void SetMeanBeamEnergy(const double&);
    void SetBeamSigma(const double&);
    void SetExcitationEnergy(const double&); 
    double SeparationEnergy();

    double r,s,Pcm_in,Pcm_out,rapidity;
    bool Calc2Body(const double&);
    bool Calc3Body();

  };

}

#endif
