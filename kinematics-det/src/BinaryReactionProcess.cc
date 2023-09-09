
#include "BinaryReactionProcess.hh"
#include "NucleonStates.hh"
#include "MassLookup.hh"

BinaryReactionProcess::BinaryReactionProcess(const G4String& processName) : 
  G4VDiscreteProcess(processName, fHadronic), 
  fScatteringEnergy(1e6) {
    
    SetProcessSubType(111);
    //SetAngDis("assets/dwba_l0.dat");
    //SetAngDis("assets/elastic.txt");
    SetAngDis("assets/state2.txt");
}

BinaryReactionProcess::~BinaryReactionProcess() {}

G4double BinaryReactionProcess::GetMeanFreePath(const G4Track& aTrack, G4double, G4ForceCondition* condition) {

  // Grab Detector construction to get access to the target volume
  const DetectorConstruction* detectorConstruction 
    = static_cast<const DetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // G4LogicalVolume* pWorldLogical = detectorConstruction->GetWorldVolume();
  G4LogicalVolume* pTargetLogical = detectorConstruction->GetTargetVolume();
  G4LogicalVolume* currentVolume = aTrack.GetStep()->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  G4double mfp = DBL_MAX;
  if(currentVolume == pTargetLogical && aTrack.GetTrackID() == 1) {
    mfp = 0.0;
  } else {
    return DBL_MAX;
  }

  *condition = NotForced;
  return mfp;
}


G4VParticleChange* BinaryReactionProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) {

  Beam.Z = aTrack.GetDefinition()->GetAtomicNumber();
  Beam.A = aTrack.GetDefinition()->GetAtomicMass();
  Beam.InvMass = MassLookup::GetInstance().FindMass(Beam.Z, Beam.A);
  Beam.M = Beam.InvMass;
  Beam.KE =  aTrack.GetKineticEnergy() / MeV;
  Beam.name = aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName();
  Beam.Pos = aTrack.GetPosition();
  Beam.x = aTrack.GetPosition().x();
  Beam.y = aTrack.GetPosition().y();
  Beam.z = aTrack.GetPosition().z();
  Beam.LV.setPx(aTrack.GetMomentum().x());
  Beam.LV.setPy(aTrack.GetMomentum().y());
  Beam.LV.setPz(aTrack.GetMomentum().z());
  Beam.LV.setE(Beam.KE + Beam.M);

  G4StepPoint* preStepPoint = aStep.GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();

  if(postStepPoint->GetStepStatus() == fGeomBoundary || Beam.name != aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName()) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
  aParticleChange.Initialize(aTrack);
  NucleonStates* states = NucleonStates::Instance();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  // Define the neutron or other light recoil particles
  // Determine if (d, d), (d, n) or (c, c)
  G4ParticleDefinition* pTargetDef;
  G4ParticleDefinition* pEjectileDef;
  G4ParticleDefinition* neutron = particleTable->FindParticle("neutron");
  G4ParticleDefinition* hydrogen = particleTable->GetIonTable()->GetIon(1, 1, 0.0);
  G4ParticleDefinition* he3 = particleTable->GetIonTable()->GetIon(2, 3, 0.0);
  G4ParticleDefinition* helium = particleTable->GetIonTable()->GetIon(2, 4, 0.0);
  G4ParticleDefinition* deuteron = particleTable->GetIonTable()->GetIon(1, 2, 0.0);
  G4ParticleDefinition* carbon = particleTable->GetIonTable()->GetIon(6, 12, 0.0);
 
  //pTargetDef = hydrogen;
  pTargetDef = deuteron;
  //pTargetDef = G4UniformRand() >= 0.5 ? hydrogen : deuteron;
  
  // if(G4UniformRand() > 0.9) {
  //   targetDef = deuteron;
  // } else {
  //   targetDef = carbon;
  // }
  
  Target.Z = pTargetDef->GetAtomicNumber();
  Target.A = pTargetDef->GetAtomicMass();
  Target.InvMass = MassLookup::GetInstance().FindMass(Target.Z, Target.A);
  Target.M = Target.InvMass;
  Target.LV.setPx(0.0);
  Target.LV.setPy(0.0);
  Target.LV.setPz(0.0);
  Target.LV.setE(Target.M);

  // G4double energy = aTrack.GetKineticEnergy() / MeV;
  // G4double cmEnergy = energy*Target.M/(Beam.M + Target.M);
 
  // pEjectileDef = G4UniformRand() >= 0.5 ? he3 : helium;
  if(false) { // elastic scattering!
    pEjectileDef = pTargetDef;
  } else if(pTargetDef == deuteron) {
    // choose neutron for (d, n) or (d, 3He)
    // pEjectileDef = G4UniformRand() >= 0.5 ? he3 : neutron;
    pEjectileDef = neutron;
  } else if(pTargetDef == hydrogen) {
    // pEjectileDef = G4UniformRand() > 0.5 ? he3 : neutron;
    pEjectileDef = helium;
  } else { // Only (c, c)
    pEjectileDef = carbon;
  }
  
  Ejectile.Z = pEjectileDef->GetAtomicNumber();
  Ejectile.A = pEjectileDef->GetAtomicMass();
  Ejectile.InvMass = MassLookup::GetInstance().FindMass(Ejectile.Z, Ejectile.A);
  Ejectile.M = Ejectile.InvMass;

  // Figure out Fragment particle charge and mass
  Fragment.Z = Beam.Z + Target.Z - Ejectile.Z;
  Fragment.A = Beam.A + Target.A - Ejectile.A;
  Fragment.Ex = states->GetExcitedLevel(Fragment.Z, Fragment.A, G4UniformRand());
  G4ParticleDefinition* pFragmentDef = particleTable->GetIonTable()->GetIon(Fragment.Z, Fragment.A, Fragment.Ex);
  Fragment.InvMass = MassLookup::GetInstance().FindMass(Fragment.Z, Fragment.A);
  Fragment.M = Fragment.InvMass;

  G4double QValue = Beam.M + Target.M - (Fragment.M + Ejectile.M + Fragment.Ex);
  G4double term1 = Beam.KE*Beam.M*Ejectile.M;
  G4double term2 = Fragment.M*(Ejectile.M + Fragment.M)*QValue;
  G4double term3 = Fragment.M*(Fragment.M + Ejectile.M - Beam.M)*Beam.KE;
  G4double gamma = std::sqrt(term1/(term2+term3));
  
  // Lab Frame Ejectile Theta, Iliadis(C.38)
  G4double ThetaCM = GetInvKinTheta();
  //G4double ThetaCM = G4UniformRand();
  // G4double ThetaCM = std::acos(1.0 - (G4UniformRand()*2.0));
  // G4double ThetaCM = std::acos(-0.5-(G4UniformRand()/2.0));
  Ejectile.Theta = std::acos((gamma + std::cos(ThetaCM))/std::sqrt(1 + gamma*gamma + 2*gamma*std::cos(ThetaCM)));
  G4double r = std::sqrt(Beam.KE*Beam.M*Ejectile.M)*std::cos(Ejectile.Theta)/(Ejectile.M+Fragment.M); 
  G4double s = (Beam.KE*(Fragment.M-Beam.M)+Fragment.M*QValue)/(Ejectile.M+Fragment.M);

  if ((r+std::sqrt(r*r+s)) > 0) { // Iliadis(C.5-6)
    Ejectile.KE = (r+std::sqrt(r*r+s))*(r+std::sqrt(r*r+s));
  } else{
    Ejectile.KE = (r-std::sqrt(r*r+s))*(r-std::sqrt(r*r+s));
  }

  Ejectile.E = Ejectile.KE + Ejectile.M;
  Ejectile.P = std::sqrt(Ejectile.E*Ejectile.E - Ejectile.M*Ejectile.M);
  Ejectile.Phi = G4UniformRand()*2.0*CLHEP::pi;
  Ejectile.LV.setPx(Ejectile.P*std::cos(Ejectile.Phi)*std::sin(Ejectile.Theta));
  Ejectile.LV.setPy(Ejectile.P*std::sin(Ejectile.Phi)*std::sin(Ejectile.Theta));
  Ejectile.LV.setPz(Ejectile.P*std::cos(Ejectile.Theta));
  Ejectile.LV.setE(Ejectile.E);
  Ejectile.Pos = Ejectile.LV.getV();

  Fragment.Theta = std::atan(std::sin(Ejectile.Theta)/(std::sqrt(Beam.M*Beam.KE/(Ejectile.M*Ejectile.KE))-std::cos(Ejectile.Theta)));
  Fragment.KE=(Ejectile.M*Ejectile.KE/Fragment.M)*std::pow(std::sin(Ejectile.Theta)/std::sin(Fragment.Theta),2);
  Fragment.E = Fragment.KE + Fragment.M;
  Fragment.LV = Beam.LV + Target.LV - Ejectile.LV;
  Fragment.Theta = Fragment.LV.theta();
  Fragment.Phi = Fragment.LV.phi();
  Fragment.Pos = Fragment.LV.getV();


  G4Track* sec1 = new G4Track(
    new G4DynamicParticle(pEjectileDef, Ejectile.Pos.unit(), Ejectile.KE*MeV),
    aTrack.GetGlobalTime(), 
    aTrack.GetPosition());
                    

  // below threshold keep it!
  G4Track* sec2 = new G4Track(
    new G4DynamicParticle(pFragmentDef, Fragment.Pos.unit(), Fragment.KE*MeV),
    aTrack.GetGlobalTime(), 
    aTrack.GetPosition());

  // Check and see if the heavy recoil has any particle thresholds
  if(!states->GetThresholds(Fragment.Z, Fragment.A).empty()) {
    
    auto threshold = states->GetThresholds(Fragment.Z, Fragment.A, G4UniformRand());

    if(Fragment.Ex > threshold.energy) {

      Decay_Heavy.Z = Fragment.Z - threshold.decay_charge;
      Decay_Heavy.A = Fragment.A - threshold.decay_mass;
      G4ParticleDefinition* pHeavyDecayDef = particleTable->GetIonTable()->GetIon(Decay_Heavy.Z, Decay_Heavy.A, 0.0);
      Decay_Heavy.InvMass = MassLookup::GetInstance().FindMass(Decay_Heavy.Z, Decay_Heavy.A);
      Decay_Heavy.M = Decay_Heavy.InvMass;

      // Get decay light Particle Definition
      Decay_Light.Z = threshold.decay_charge;
      Decay_Light.A = threshold.decay_mass;
      G4ParticleDefinition* pLightDecayDef = particleTable->GetIonTable()->GetIon(Decay_Light.Z, Decay_Light.A, 0.0);
      Decay_Light.InvMass = MassLookup::GetInstance().FindMass(Decay_Light.Z, Decay_Light.A);
      Decay_Light.M = Decay_Light.InvMass;

      G4ThreeVector boost = Fragment.LV.boostVector();
      Fragment.LV.boost(-boost);
      G4double Ecm = (Fragment.LV.e()*Fragment.LV.e() + 
        Decay_Light.M*Decay_Light.M - Decay_Heavy.M*Decay_Heavy.M) /(2.0*Fragment.LV.e());

      G4double Pcm = std::sqrt(Ecm*Ecm - Decay_Light.M*Decay_Light.M);

      Decay_Light.Theta = std::acos((G4UniformRand()*2.0) - 1.0); //in CM frame
      Decay_Light.Phi = G4UniformRand()*2*CLHEP::pi;
      Decay_Light.LV.setPx(Pcm*std::cos(Decay_Light.Phi)*std::sin(Decay_Light.Theta));
      Decay_Light.LV.setPy(Pcm*std::sin(Decay_Light.Phi)*std::sin(Decay_Light.Theta));
      Decay_Light.LV.setPz(Pcm*std::cos(Decay_Light.Theta));
      Decay_Light.LV.setE(Ecm);

      Decay_Heavy.LV = Fragment.LV - Decay_Light.LV;

      // Boost back to the lab frame
      Fragment.LV.boost(boost);
      Decay_Light.LV.boost(boost);
      Decay_Heavy.LV.boost(boost); 

      Decay_Light.E = Decay_Light.LV.e();
      Decay_Light.KE = Decay_Light.E - Decay_Light.M;
      Decay_Light.Theta = Decay_Light.LV.theta();
      Decay_Light.Phi = Decay_Light.LV.phi();
      Decay_Light.Pos = Decay_Light.LV.getV();

      Decay_Heavy.E = Decay_Heavy.LV.e();
      Decay_Heavy.KE = Decay_Heavy.E - Decay_Heavy.M;
      Decay_Heavy.Theta = Decay_Heavy.LV.theta();
      Decay_Heavy.Phi = Decay_Heavy.LV.phi();
      Decay_Heavy.Pos = Decay_Heavy.LV.getV();

      G4Track* pDecayTrackHeavy = new G4Track(
        new G4DynamicParticle(pHeavyDecayDef, Decay_Heavy.Pos.unit(), Decay_Heavy.KE*MeV),
        aTrack.GetGlobalTime(), 
        aTrack.GetPosition());

      G4Track* pDecayTrackLight = new G4Track(
        new G4DynamicParticle(pLightDecayDef, Decay_Light.Pos.unit(), Decay_Light.KE*MeV),
        aTrack.GetGlobalTime(), 
        aTrack.GetPosition());

      aParticleChange.SetNumberOfSecondaries(3);
      aParticleChange.AddSecondary(sec1);
      aParticleChange.AddSecondary(pDecayTrackHeavy);
      aParticleChange.AddSecondary(pDecayTrackLight);

      sec1->SetUserInformation(
        new TrackingInformation(
        ThetaCM, 
        Ejectile.Theta, 
        Fragment.Theta,
        Decay_Light.Theta,
        Decay_Heavy.Theta,
        Ejectile.KE,
        Fragment.KE, 
        Decay_Light.KE,
        Decay_Heavy.KE,
        QValue, 
        Fragment.Ex));
    }
  } else { // No Thresholds!

    aParticleChange.SetNumberOfSecondaries(2);
    aParticleChange.AddSecondary(sec1);
    aParticleChange.AddSecondary(sec2);
  
    sec1->SetUserInformation(
      new TrackingInformation(
      ThetaCM, 
      Ejectile.Theta, 
      Fragment.Theta,
      0.0, 
      0.0,
      Ejectile.KE,
      Fragment.KE, 
      0.0,
      0.0,
      QValue, 
      Fragment.Ex));
  }

  aParticleChange.ProposeEnergy(0.);
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  

  return &aParticleChange;
}



void BinaryReactionProcess::StartTracking(G4Track* track) {
  // Apply base class actions
  G4VProcess::StartTracking(track);	

  // To make interaction happen anywhere from beam energy to 0
  // fScatteringEnergy = track->GetKineticEnergy()*G4UniformRand()/MeV;

  // To make interaction happen anywhere from beam energy to beam energy - 20 MeV
  G4double beamEnergy = track->GetKineticEnergy()/MeV;
  G4double beamEnergyMin = beamEnergy/2.;

  fScatteringEnergy = (beamEnergy - beamEnergyMin)*G4UniformRand() + beamEnergyMin;
}




G4VParticleChange* BinaryReactionProcess::Decay(const G4Track& aTrack, G4int lightCharge, G4int lightMass, G4int heavyCharge, G4int heavyMass) {
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  // Setup Particle 1
  G4DynamicParticle* particle1 = new G4DynamicParticle;
  G4ParticleDefinition* particle1Def;

  if(lightCharge == 0 && lightMass == 1) {
    G4String particleName;
    particle1Def = particleTable->FindParticle(particleName="neutron");
  } else {
    
    if(particleTable->GetIonTable()->FindIon(lightCharge, lightMass, 0.)) {
      particle1Def = particleTable->GetIonTable()->FindIon(lightCharge, lightMass, 0.);
    } else {
      particle1Def = particleTable->GetIonTable()->GetIon(lightCharge, lightMass, 0.);
    }
  }
  
  particle1->SetDefinition(particle1Def);
  G4double particle1Mass = particle1Def->GetPDGMass()/CLHEP::amu_c2;

  // Setup Particle 2
  G4DynamicParticle* particle2 = new G4DynamicParticle;
  G4ParticleDefinition* particle2Def = G4IonTable::GetIonTable()->GetIon(heavyCharge, heavyMass, 0.);
  particle2->SetDefinition(particle2Def);
  G4double particle2Mass = particle2Def->GetPDGMass()/CLHEP::amu_c2;

  G4double qValue = aTrack.GetDynamicParticle()->GetDefinition()->GetPDGMass() - (particle1Def->GetPDGMass() + particle2Def->GetPDGMass());
  G4double cmEnergy = qValue;

  if(cmEnergy < 0.0) return &aParticleChange; // Below the threshold

  // Generate random CM Angles
  G4double cmTheta = CLHEP::pi*G4UniformRand();; // 0 to pi
  G4double cmPhi = 2.0*CLHEP::pi*G4UniformRand(); // 0 to 2pi

  G4double p1 = sqrt(2.0*particle1->GetMass()*cmEnergy*particle2Mass/(particle1Mass + particle2Mass));
  G4double p2 = sqrt(2.0*particle2->GetMass()*cmEnergy*particle1Mass/(particle1Mass + particle2Mass));

  // Get momentum directions
  G4ThreeVector p_new_1 = G4ThreeVector(p1*sin(cmTheta)*sin(cmPhi), p1*sin(cmTheta)*cos(cmPhi), p1*cos(cmTheta));
  G4ThreeVector p_new_2 = -p_new_1;
  G4ThreeVector p_parent = aTrack.GetMomentum();
  p_new_1 += p_parent*(particle1Mass/(particle1Mass + particle2Mass));
  p_new_2 += p_parent*(particle2Mass/(particle1Mass + particle2Mass));
  particle1->SetMomentum(p_new_1);
  particle2->SetMomentum(p_new_2);

  G4double total_mom_1 = p_new_1.getR();
  G4double total_mom_2 = p_new_2.getR();
  particle1->SetKineticEnergy((total_mom_1*total_mom_1)/(2.0*particle1->GetMass()));
  particle2->SetKineticEnergy((total_mom_2*total_mom_2)/(2.0*particle2->GetMass()));

  G4Track* sec1 = new G4Track(particle1, aTrack.GetGlobalTime(), aTrack.GetPosition());
  G4Track* sec2 = new G4Track(particle2, aTrack.GetGlobalTime(), aTrack.GetPosition());

  aParticleChange.AddSecondary(sec1);
  aParticleChange.AddSecondary(sec2);
  aParticleChange.ProposeEnergy(0.0);
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  return &aParticleChange;
}

