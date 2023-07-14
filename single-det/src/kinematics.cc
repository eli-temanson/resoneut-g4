
//==================================================================================
// Physics Calculations 
// 0 + 1 -> 2 + 3* -> 2 + (4+5)
// Beam + Target -> Ejectile + Fragment* -> Ejectile + (Decay_Light + Decay_Heavy)
// All Everything should be in MeV via natural units
//
// Author: Eli Temanson
// Date: May 25, 2022
//==================================================================================

#ifndef kinematics_cc
#define kinematics_cc

#include "kinematics.hh"
#include "mass_lookup.hh"

// using json-c++ reader
#include <nlohmann/json.hpp>
using json = nlohmann::json;


namespace kinematics {


  void Kinematics::SetMeanBeamEnergy(const double& e) { 
    MeanBeamEnergy = e;
  }
  void Kinematics::SetBeamSigma(const double& esig) { 
    BeamSigma=esig;
  }
  void Kinematics::SetExcitationEnergy(const double& e) { 
    ExcEnergy = e; 
  }
  double Kinematics::SeparationEnergy() { 
    Sp = Decay_Heavy.M + Decay_Light.M - Fragment.M; 
    return Sp;
  }

  //Set Primary Reaction and automatically add one decay channel
  void Kinematics::SetReaction(int nucleus[][2]) {
    Beam.Z = nucleus[0][0];
    Beam.A = nucleus[0][1];
    Beam.M = MassLookup::GetInstance().FindMass(Beam.Z, Beam.A);

    Target.Z = nucleus[1][0];
    Target.A = nucleus[1][1];
    Target.M = MassLookup::GetInstance().FindMass(Target.Z, Target.A);

    Ejectile.Z = nucleus[2][0];
    Ejectile.A = nucleus[2][1];
    Ejectile.M = MassLookup::GetInstance().FindMass(Ejectile.Z, Ejectile.A);

    Fragment.Z = nucleus[3][0];
    Fragment.A = nucleus[3][1];
    Fragment.InvMass = MassLookup::GetInstance().FindMass(Fragment.Z, Fragment.A);
    Fragment.M = Fragment.InvMass;

    Decay_Heavy.Z = nucleus[4][0];
    Decay_Heavy.A = nucleus[4][1];
    Decay_Heavy.M = MassLookup::GetInstance().FindMass(Decay_Heavy.Z, Decay_Heavy.A);

    Decay_Light.Z = nucleus[5][0];
    Decay_Light.A = nucleus[5][1];
    Decay_Light.M = MassLookup::GetInstance().FindMass(Decay_Light.Z, Decay_Light.A);
  }


  bool Kinematics::Calc2Body(const double& th) { 
    ThetaCM = th;
    QValue = Beam.M + Target.M - (Fragment.M + Ejectile.M + ExcEnergy);
    Beam.KE = Rndm->Gaus(MeanBeamEnergy, BeamSigma); 

    term1 = Beam.KE*Beam.M*Ejectile.M;
    term2 = Fragment.M*(Ejectile.M + Fragment.M)*QValue;
    term3 = Fragment.M*(Fragment.M + Ejectile.M - Beam.M)*Beam.KE;
    gamma = std::sqrt(term1/(term2+term3));

    // Lab Frame Ejectile Theta, Iliadis(C.38)
    Ejectile.Theta = std::acos((gamma + std::cos(ThetaCM))/std::sqrt(1 + gamma*gamma + 2*gamma*std::cos(ThetaCM)));
    r = std::sqrt(Beam.KE*Beam.M*Ejectile.M)*std::cos(Ejectile.Theta)/(Ejectile.M+Fragment.M); 
    s = (Beam.KE*(Fragment.M-Beam.M)+Fragment.M*QValue)/(Ejectile.M+Fragment.M);

    if ((r+std::sqrt(r*r+s)) > 0) // Iliadis(C.5-6)
    {
      Ejectile.KE = (r+std::sqrt(r*r+s))*(r+std::sqrt(r*r+s));
    }else{
      Ejectile.KE = (r-std::sqrt(r*r+s))*(r-std::sqrt(r*r+s));
    }

    // Ejectile.P = sqrt(Ejectile.KE*(Ejectile.KE + 2.0*Ejectile.M));
    Ejectile.E = Ejectile.KE + Ejectile.M;
    Ejectile.P = std::sqrt(Ejectile.E*Ejectile.E - Ejectile.M*Ejectile.M);
    Ejectile.Phi = Rndm->Uniform(0.0,2*TMath::Pi());
    Ejectile.Px=Ejectile.P*std::cos(Ejectile.Phi)*std::sin(Ejectile.Theta);
    Ejectile.Py=Ejectile.P*std::sin(Ejectile.Phi)*std::sin(Ejectile.Theta);
    Ejectile.Pz=Ejectile.P*std::cos(Ejectile.Theta);
    Ejectile.LV.SetPxPyPzE(Ejectile.Px, Ejectile.Py, Ejectile.Pz, Ejectile.E);

    // Assign Beam assuming it's on axis
    Beam.E = Beam.KE + Beam.M;
    Beam.P = sqrt(Beam.E*Beam.E - Beam.M*Beam.M);
    Beam.LV.SetPxPyPzE(0,0,Beam.P,Beam.E);
    
    // Assign Stationary target
    Target.LV.SetPxPyPzE(0,0,0,Target.M);

    // There are a few ways to now calculate the kinematics of the fragment 
    Fragment.Theta = std::atan(std::sin(Ejectile.Theta)/(std::sqrt(Beam.M*Beam.KE/(Ejectile.M*Ejectile.KE))-std::cos(Ejectile.Theta)));
    Fragment.KE=(Ejectile.M*Ejectile.KE/Fragment.M)*std::pow(std::sin(Ejectile.Theta)/std::sin(Fragment.Theta),2);
    Fragment.E = Fragment.KE + Fragment.M;
    Fragment.LV = Beam.LV + Target.LV - Ejectile.LV;
    Fragment.Theta = Fragment.LV.Theta();
    Fragment.Phi = Fragment.LV.Phi();

    // Fragment.E = Fragment.LV.E();
    // Fragment.KE = Fragment.E - Fragment.M - ExcEnergy;
    // Fragment.Phi = Rndm->Uniform(0.0,2*TMath::Pi());
    // Fragment.P = sqrt(Fragment.KE*(Fragment.KE + 2.0*Fragment.M));
    // Fragment.Px=Fragment.P*cos(Fragment.Phi)*sin(Fragment.Theta);
    // Fragment.Py=Fragment.P*sin(Fragment.Phi)*sin(Fragment.Theta);
    // Fragment.Pz=Fragment.P*cos(Fragment.Theta);
    // Fragment.LV.SetPxPyPzE(Fragment.Px, Fragment.Py, Fragment.Pz, Fragment.E);

    return true;
  }


  // Now calculate 3 body break up of the fragment in CM frame
  bool Kinematics::Calc3Body() {
    
    TVector3 boost = Fragment.LV.BoostVector();
    Fragment.LV.Boost(-boost); //Boost the Fragment to it's center of mass
    
    Ecm = (Fragment.LV.E()*Fragment.LV.E() + 
          Decay_Light.M*Decay_Light.M - 
          Decay_Heavy.M*Decay_Heavy.M) /(2.0*Fragment.LV.E());

    Pcm = std::sqrt(Ecm*Ecm - Decay_Light.M*Decay_Light.M);

    Decay_Light.Theta = std::acos(Rndm->Uniform(-1,1)); //in CM frame
    Decay_Light.Phi = Rndm->Uniform(0,2*TMath::Pi());

    Decay_Light.LV.SetPx(Pcm*std::cos(Decay_Light.Phi)*std::sin(Decay_Light.Theta));
    Decay_Light.LV.SetPy(Pcm*std::sin(Decay_Light.Phi)*std::sin(Decay_Light.Theta));
    Decay_Light.LV.SetPz(Pcm*std::cos(Decay_Light.Theta));
    Decay_Light.LV.SetE(Ecm);

    Decay_Heavy.LV = Fragment.LV - Decay_Light.LV;

    Fragment.LV.Boost(boost); // Boost back to the lab frame
    Decay_Light.LV.Boost(boost); // Boost back to the lab frame
    Decay_Heavy.LV.Boost(boost); // Boost back to the lab frame

    Decay_Light.E = Decay_Light.LV.E();
    Decay_Light.KE = Decay_Light.E - Decay_Light.M;
    Decay_Light.Theta = Decay_Light.LV.Theta();
    Decay_Light.Phi = Decay_Light.LV.Phi();

    Decay_Heavy.E = Decay_Heavy.LV.E();
    Decay_Heavy.KE = Decay_Heavy.E - Decay_Heavy.M;
    Decay_Heavy.Theta = Decay_Heavy.LV.Theta();
    Decay_Heavy.Phi = Decay_Heavy.LV.Phi();

    Decay_Light.Pos.SetMagThetaPhi(1.0,Decay_Light.Theta,Decay_Light.Phi);

    return true;
  }



}

#endif




