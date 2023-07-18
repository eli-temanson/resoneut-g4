
#include "BinaryReactionProcess.hh"
#include "NucleonStates.hh"
#include "mass_lookup.hh"

BinaryReactionProcess::BinaryReactionProcess(const G4String& processName) : 
  G4VDiscreteProcess(processName, fHadronic), 
  fScatteringEnergy(1e6) {
    SetProcessSubType(111);
}

BinaryReactionProcess::~BinaryReactionProcess() {}

G4double BinaryReactionProcess::GetMeanFreePath(const G4Track& aTrack, G4double, G4ForceCondition* condition) {

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

  // Grab Detector construction to get access to the target volume
  const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume* pWorldLogical = detectorConstruction->GetWorldVolume();
  G4LogicalVolume* pTargetLogical = detectorConstruction->GetTargetVolume();
  G4LogicalVolume* currentVolume = aTrack.GetStep()->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  G4double mfp = DBL_MAX;
  if(currentVolume == pTargetLogical && aTrack.GetTrackID() == 1) {
    mfp = 0.0;
  } 

  // G4double energy = aTrack.GetKineticEnergy()/MeV;
  // G4int particleMass = aTrack.GetDefinition()->GetAtomicMass();
  // G4int particleCharge = aTrack.GetDefinition()->GetAtomicNumber();
  // G4String excitedname = aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName();
  // G4double position = aTrack.GetPosition().z();
  // Mean Free Path
  // G4double mfp = (
  //   energy <= fScatteringEnergy &&
  //   aTrack.GetTrackID() == 1 &&
  //   (currentVolume == pTargetLogical ||
  //   (currentVolume == pWorldLogical && position > -0.0066 && position < 0.1))) ? 0. : DBL_MAX;

  // G4double mfp = (
  //   energy <= fScatteringEnergy &&
  //   aTrack.GetTrackID() == 1 && 
  //   currentVolume == pTargetLogical
  //   ) ? 0.0 : DBL_MAX;
  

  // else if (excitedname.find("11C[") && aTrack.GetTrackID() == 3) {
  //   mfp = 0.0;
  // }

  // // Look at excited name and see if it's in an excited state
  // size_t pos = excitedname.find('[');
  // double measuredExcitedEnergy = 0.0;
  // G4String beamNameEnergy = "";

  // if(pos > 1000) {
  //   measuredExcitedEnergy = 0.0;
  // } else {
  //   beamNameEnergy = excitedname.substr(pos + 1, std::string::npos);
  //   beamNameEnergy.pop_back();
  //   measuredExcitedEnergy = std::atof(beamNameEnergy.c_str())/1000.0;
  // }
  // // Check if above threshold and needs to decay
  // NucleonStates* states = NucleonStates::Instance();
  // auto thresholds = states->GetThresholds(particleCharge, particleMass);
  // if(measuredExcitedEnergy > 0 && !thresholds.empty()) {
  //   if(measuredExcitedEnergy > thresholds[0].energy) {
  //     mfp = 0.;
  //   }
  // } 
  // if(excitedname == "Be8") {
  //   mfp = 0.;
  // }

  *condition = NotForced;

  return mfp;
}



G4VParticleChange* BinaryReactionProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) {

  // // Determine the beam particle name
  // G4String incomingParticleName = aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName();
  // size_t pos = incomingParticleName.find('[');
  // G4double measuredExcitedEnergy = 0.0;
  // G4String incomingParticleNameEnergy = "";

  // // Check if the beam has an excited state
  // if(pos > 100) {
  //   measuredExcitedEnergy = 0.;
  // } else {
  //   incomingParticleNameEnergy = incomingParticleName.substr(pos + 1, std::string::npos);
  //   incomingParticleNameEnergy.pop_back();
  //   measuredExcitedEnergy = std::atof(incomingParticleNameEnergy.c_str())/1000.;
  // }

  // NucleonStates* states = NucleonStates::Instance();

  // // Check and see if the beam has any particle thresholds
  // auto thresholds = states->GetThresholds(aTrack.GetParticleDefinition()->GetAtomicNumber(), aTrack.GetParticleDefinition()->GetAtomicMass());

  // if(!thresholds.empty()) {
  //   auto threshold = thresholds[0];
  //   if(measuredExcitedEnergy > threshold.energy) {
  //     G4int lightCharge = aTrack.GetDynamicParticle()->GetDefinition()->GetAtomicNumber() - threshold.decay_charge;
  //     G4int lightMass = aTrack.GetDynamicParticle()->GetDefinition()->GetAtomicMass() - threshold.decay_mass;
  //     return Decay(aTrack, lightCharge, lightMass, threshold.decay_charge, threshold.decay_mass);
  //   }
  // }

  // G4StepPoint* preStepPoint = aStep.GetPreStepPoint();
  // G4StepPoint* postStepPoint = aStep.GetPostStepPoint();

  // // idk what this is...
  // if(postStepPoint->GetStepStatus() == fGeomBoundary) {
  //   return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  // }
  // const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>
  //     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // G4LogicalVolume* pWorldLogical = detectorConstruction->GetWorldVolume();

  // aParticleChange.Initialize(aTrack);

  // G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // G4String particleName;
  // G4ParticleDefinition* neutron = particleTable->FindParticle(particleName="neutron");
  // G4ParticleDefinition* deutron = particleTable->GetIonTable()->GetIon(1, 2, 0.0);
  // G4ParticleDefinition* carbon = particleTable->GetIonTable()->GetIon(6, 12, 0.0);

  // // Get beam properties
  // G4int beamCharge = aTrack.GetParticleDefinition()->GetAtomicNumber();
  // G4int beamMass = aTrack.GetParticleDefinition()->GetAtomicMass();
  // G4double beamMassPDG = aTrack.GetParticleDefinition()->GetPDGMass();

  // G4int targetCharge, targetMass;
  // G4int lightRecoilCharge, lightRecoilMass;
  // G4int heavyRecoilCharge, heavyRecoilMass;
  // G4double targetMassPDG, lightRecoilMassPDG, heavyRecoilMassPDG;

  // Determine if (d, d), (d, n) or (c, c)
  // 90% of the time choose the deutron as the target
  // G4ParticleDefinition* targetDef = G4UniformRand() > 0.9 ? carbon : deutron;
  // G4ParticleDefinition* targetDef = deutron;

  // targetCharge = targetDef->GetAtomicNumber();
  // targetMass = targetDef->GetAtomicMass();
  // targetMassPDG = targetDef->GetPDGMass();

  // G4double energy = aTrack.GetKineticEnergy()/MeV;
  // G4double cmEnergy = energy*targetMassPDG/(beamMassPDG + targetMassPDG);

  // // Define the neutron or other light recoil particles
  // G4ParticleDefinition* lightRecoilDef;

  // // 80% of the time choose neutron for (d, n) instead of (d, d)
  // if(targetDef == deutron) {
  //   // lightRecoilDef = G4UniformRand() > 0.8 ? deutron : neutron;
  //   lightRecoilDef = neutron;
  // } else { // Only (c, c)
  //   lightRecoilDef = carbon;
  // }

  // lightRecoilCharge = lightRecoilDef->GetAtomicNumber();
  // lightRecoilMass = lightRecoilDef->GetAtomicMass();
  // lightRecoilMassPDG = lightRecoilDef->GetPDGMass();

  // // Figure out heavy particle charge and mass
  // heavyRecoilCharge = beamCharge + targetCharge - lightRecoilCharge;
  // heavyRecoilMass = beamMass + targetMass - lightRecoilMass;

  // // Sample on excited states
  // G4double exProbability = G4UniformRand();
  // G4double excitedEnergy = states->GetExcitedLevel(heavyRecoilCharge, heavyRecoilMass, exProbability);

  // // Get Heavy Ion Particle Definition
  // G4ParticleDefinition* heavyRecoilDef = particleTable->GetIonTable()->GetIon(heavyRecoilCharge, heavyRecoilMass, excitedEnergy);
  // heavyRecoilMassPDG = heavyRecoilDef->GetPDGMass();

  // If the light recoil is heavier than the heavy recoil, switch them
  // if(lightRecoilMass > heavyRecoilMass) {
  //   std::swap(lightRecoilCharge, heavyRecoilCharge);
  //   std::swap(lightRecoilMass, heavyRecoilMass);
  //   std::swap(lightRecoilMassPDG, heavyRecoilMassPDG);
  //   std::swap(lightRecoilDef, heavyRecoilDef);
  // }

  // G4double qValue = beamMassPDG + targetMassPDG - (lightRecoilMassPDG + heavyRecoilMassPDG);

  // // Check if reaction is possible
  // if(cmEnergy + qValue < 0) {
  //   G4cout << "Not enough energy: " << incomingParticleName << "; Energy: " << energy << "; CM Energy: " << cmEnergy << G4endl;
  //   G4cout << "\t Q Value: " << qValue << "; Vertex Location: " << aTrack.GetPosition().z() << G4endl;
  //   return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  // }

  // G4double totalEnergy = energy + qValue;

  // G4double B = beamMassPDG*lightRecoilMassPDG/(beamMassPDG + targetMassPDG)/(lightRecoilMassPDG + heavyRecoilMassPDG)*
  //             (energy/totalEnergy);
  // G4double D = targetMassPDG*heavyRecoilMassPDG/(beamMassPDG + targetMassPDG)/(lightRecoilMassPDG + heavyRecoilMassPDG)*
  //             (1.0 + beamMassPDG/targetMassPDG*qValue/totalEnergy);

  // G4double maxAngle = (B < D) ? M_PI : asin(sqrt(D/B));
  // // G4double arg = (1. - cos(maxAngle))*G4UniformRand() + cos(maxAngle);
  // // G4double pAngleLightLab = acos(arg);
  // G4double pAngleLightLab = maxAngle*G4UniformRand();
  // G4double aAngleLightLab = 2.0*M_PI*G4UniformRand();

  // G4ThreeVector lightVector(sin(pAngleLightLab)*cos(aAngleLightLab), sin(pAngleLightLab)*sin(aAngleLightLab),
  //                           cos(pAngleLightLab));
  // G4ThreeVector beamVector(0., 0., 1.);

  // G4double lightEnergyLab = (B <= D) ? totalEnergy*B*pow(cos(pAngleLightLab) + sqrt(D/B - sin(pAngleLightLab)*sin(pAngleLightLab)), 2.) :
  //                           totalEnergy*B*pow(cos(pAngleLightLab) - sqrt(D/B - sin(pAngleLightLab)*sin(pAngleLightLab)), 2.);
  // G4double lightEnergyLab2 = (B <= D) ? totalEnergy*B*pow(cos(pAngleLightLab + 0.001) + sqrt(D/B - sin(pAngleLightLab + 0.001)*sin(pAngleLightLab + 0.001)), 2.) :
  //                           totalEnergy*B*pow(cos(pAngleLightLab + 0.001) - sqrt(D/B - sin(pAngleLightLab + 0.001)*sin(pAngleLightLab + 0.001)), 2.);

  // G4double heavyEnergyLab = totalEnergy - lightEnergyLab;

  // // Make sure the energies of both recoils are greater than 0
  // if(lightEnergyLab < 0 || heavyEnergyLab < 0) {
  //   G4cout << lightEnergyLab << '\t' << heavyEnergyLab << G4endl;
  // }

  // G4double val = sqrt(lightEnergyLab/totalEnergy/D)*sin(pAngleLightLab);
  // G4double val2 = sqrt(lightEnergyLab2/totalEnergy/D)*sin(pAngleLightLab + 0.001);
  // G4double pAngleLightCM = (val2 > val) ? asin(val) : M_PI - asin(val);
  // G4double aAngleLightCM = aAngleLightLab;

  // G4double pAngleHeavyLab = asin(sqrt(lightRecoilMassPDG/(heavyRecoilMassPDG)*
  //   lightEnergyLab/heavyEnergyLab)*sin(pAngleLightLab));
  // G4ThreeVector heavyVector(-1.*sin(pAngleHeavyLab)*cos(aAngleLightLab), -1.*sin(pAngleHeavyLab)*sin(aAngleLightLab),
  //                           cos(pAngleHeavyLab));
  // G4double pAngleHeavyCM = M_PI - pAngleLightCM;

  // G4ThreeVector momentumDirection = aTrack.GetMomentumDirection();
  // G4ThreeVector v = beamVector.cross(momentumDirection);
  // G4double rotAngle = acos(momentumDirection.z());

  // G4ThreeVector lightLab(sin(pAngleLightLab)*cos(aAngleLightLab),
	// 		 sin(pAngleLightLab)*sin(aAngleLightLab),
	// 		 cos(pAngleLightLab));
  // lightLab = lightLab.rotate(v, rotAngle);

  // G4ThreeVector heavyLab(-1.*sin(pAngleHeavyLab)*cos(aAngleLightLab),
  //      -1.*sin(pAngleHeavyLab)*sin(aAngleLightLab),
	// 		 cos(pAngleHeavyLab));
  // heavyLab = heavyLab.rotate(v, rotAngle);


  // =================================================
  G4StepPoint* preStepPoint = aStep.GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();

  if(postStepPoint->GetStepStatus() == fGeomBoundary || Beam.name != aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName()) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
  aParticleChange.Initialize(aTrack);
  NucleonStates* states = NucleonStates::Instance();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  // Determine if (d, d), (d, n) or (c, c)
  G4ParticleDefinition* pTargetDef;
  G4ParticleDefinition* deutron = particleTable->GetIonTable()->GetIon(1, 2, 0.0);
  G4ParticleDefinition* carbon = particleTable->GetIonTable()->GetIon(6, 12, 0.0);
  pTargetDef = deutron;

  // if(G4UniformRand() > 0.9) {
  //   targetDef = deutron;
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

  G4double energy = aTrack.GetKineticEnergy() / MeV;
  G4double cmEnergy = energy*Target.M/(Beam.M + Target.M);

  // Define the neutron or other light recoil particles
  G4ParticleDefinition* pEjectileDef;
  G4String particleName;
  G4ParticleDefinition* neutron = particleTable->FindParticle(particleName="neutron");

  // 80% of the time choose neutron for (d, n) instead of (d, d)
  if(pTargetDef == deutron) {
    // lightRecoilDef = G4UniformRand() > 0.8 ? deutron : neutron;
    pEjectileDef = neutron;
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
  G4double ThetaCM = std::acos(G4UniformRand()*2.0 - 1.0);
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
  Ejectile.Phi = G4UniformRand()*2.0*M_PI;
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
                    
  // sec1->SetUserInformation(
  //   new TrackingInformation(
  //   energy, 
  //   cmEnergy, 
  //   pAngleLightCM,
  //   pAngleLightLab, 
  //   aAngleLightCM, 
  //   pAngleHeavyCM,
  //   pAngleHeavyLab,
  //   lightEnergyLab,
  //   heavyEnergyLab, 
  //   aTrack.GetPosition(), 
  //   qValue, 
  //   excitedEnergy,
  //   lightRecoilDef, 
  //   heavyRecoilDef));

  sec1->SetUserInformation(
    new TrackingInformation(
    energy, 
    cmEnergy, 
    0.0,
    Ejectile.Theta, 
    0.0, 
    0.0,
    Fragment.Theta,
    Ejectile.KE,
    Fragment.KE, 
    aTrack.GetPosition(), 
    QValue, 
    Fragment.Ex,
    pEjectileDef, 
    pFragmentDef));

  // below threshold keep it!
  G4Track* sec2 = new G4Track(
    new G4DynamicParticle(pFragmentDef, Fragment.Pos.unit(), Fragment.KE*MeV),
    aTrack.GetGlobalTime(), 
    aTrack.GetPosition());

  // Check and see if the heavy recoil has any particle thresholds
  auto heavyThresholds = states->GetThresholds(Fragment.Z, Fragment.A);

  if(!heavyThresholds.empty()) {
    auto threshold = heavyThresholds[0];
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

      Decay_Light.Theta = std::acos(G4UniformRand()*2.0 - 1.0); //in CM frame
      Decay_Light.Phi = G4UniformRand()*2*M_PI;
      Decay_Light.LV.setPx(Pcm*std::cos(Decay_Light.Phi)*std::sin(Decay_Light.Theta));
      Decay_Light.LV.setPy(Pcm*std::sin(Decay_Light.Phi)*std::sin(Decay_Light.Theta));
      Decay_Light.LV.setPz(Pcm*std::cos(Decay_Light.Theta));
      Decay_Light.LV.setE(Ecm);

      Decay_Heavy.LV = Fragment.LV - Decay_Light.LV;

      Fragment.LV.boost(boost); // Boost back to the lab frame
      Decay_Light.LV.boost(boost); // Boost back to the lab frame
      Decay_Heavy.LV.boost(boost); // Boost back to the lab frame

      Decay_Light.E = Decay_Light.LV.e();
      Decay_Light.KE = Decay_Light.E - Decay_Light.M;
      Decay_Light.Theta = Decay_Light.LV.theta();
      Decay_Light.Phi = Decay_Light.LV.phi();
      // Decay_Light.Pos.setMag(1.0);
      // Decay_Light.Pos.setTheta(Decay_Light.Theta);
      // Decay_Light.Pos.setPhi(Decay_Light.Phi);
      Decay_Light.Pos = Decay_Light.LV.getV();

      Decay_Heavy.E = Decay_Heavy.LV.e();
      Decay_Heavy.KE = Decay_Heavy.E - Decay_Heavy.M;
      Decay_Heavy.Theta = Decay_Heavy.LV.theta();
      Decay_Heavy.Phi = Decay_Heavy.LV.phi();
      Decay_Heavy.Pos = Decay_Heavy.LV.getV();
      // Decay_Heavy.Pos.setMag(1.0);
      // Decay_Heavy.Pos.setTheta(Decay_Heavy.Theta);
      // Decay_Heavy.Pos.setPhi(Decay_Heavy.Phi);

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

    }
  } else { // No Thresholds!
    aParticleChange.SetNumberOfSecondaries(2);
    aParticleChange.AddSecondary(sec1);
    aParticleChange.AddSecondary(sec2);
  }

  aParticleChange.ProposeEnergy(0.);
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  

  return &aParticleChange;
}



void BinaryReactionProcess::StartTracking(G4Track* track) {
  G4VProcess::StartTracking(track);	// Apply base class actions

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
  G4double cmTheta = M_PI*G4UniformRand();; // 0 to pi
  G4double cmPhi = 2.0*M_PI*G4UniformRand(); // 0 to 2pi

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

