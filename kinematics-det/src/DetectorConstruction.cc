//===================================================================
//  Detector Construction for Geant4 Simulation
//
//  Inside of the Construct Function, we first define the World. Each
//  geometric component in geant4 has the following steps to define,
//    > G4Material
//    > Geometry, G4Box, G4Tubs etc.
//    > G4LogicalVolume
//    > G4VPhysicalVolume
//
//  The physics properties of each G4Material can be set using the 
//    "G4MaterialPropertiesTable" where below we give scintillation
//    properties to para-Terphenyl.
//
//  Optionally you can make the detector defined below a sensitive 
//    detector. See "src/SensitiveDetector.cc". First the G4LogicalVolume
//    of the detector must be defined in the "DetectorConstruction.hh" 
//    header so it can called and added to the G4SDManager inside the
//    ConstructSDandField function.
//
//  Class includes:
//    > Construct -> returns the "physical world"
//    > ConstructSDandField
//
//  Date of Last Modified: 12/15/2021
//  Author: Eli Temanson
//===================================================================
#include "DetectorConstruction.hh"
// #include "InputReader.hh"

#include "G4SubtractionSolid.hh"
#include "G4UserLimits.hh"

DetectorConstruction::DetectorConstruction() : 
  G4VUserDetectorConstruction() 
{
  // InputReader* reader = InputReader::Instance();
  // std::cout << reader->GetFileMacro() << std::endl;
}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  //
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;
  
  //===============================================================================
  // World material
  //
  G4Material* pWorldMat = nist->FindOrBuildMaterial("G4_Galactic");

  // World geometry
  //  
  G4Box* pSolidWorld = new G4Box("World",0.5*m,0.5*m,1.5*m);

  // World logic definition
  //
  pWorldLogic = new G4LogicalVolume(
    pSolidWorld, // the geometry/solid 
    pWorldMat,   // the material
    "World");	   // the name

  // Set world invisible
  //
  pWorldLogic->SetVisAttributes(G4VisAttributes::GetInvisible()); 

  // The creation of the physical world
  //
  G4VPhysicalVolume* pPhysWorld = new G4PVPlacement(
    0,	                // no rotation
  	G4ThreeVector(),	  // at (0,0,0)
    pWorldLogic,        // its logical volume
    "World",            // its name
    0,                  // its mother  volume
    false,              // no boolean operation
    0,                  // copy number
    checkOverlaps);     // overlaps checking


  //===============================================================================
  // S1 detector
  
  // Material
  //
  G4Material* pSiMat = nist->FindOrBuildMaterial("G4_Si");

  // Geometry
  //  
  G4Tubs* pS1Geo = new G4Tubs(
    "S1Geo",
    24.0*mm,
    48.0*mm,
    1000.0*um, // thickness
    0.0,
    2*CLHEP::pi);

  // logic definition
  //
  pS1Logical = new G4LogicalVolume(
    pS1Geo, // the geometry/solid 
    pSiMat,   // the material
    "S1Logic");	   // the name

  new G4PVPlacement(0,	//no rotation
    G4ThreeVector(0,0,125.0*mm),	  // the center at (0,0,0)
    pS1Logical,        // the logical volume
    "S1Logic",  // the name
    pWorldLogic,        // the mother (logical) volume
    false,              // no boolean operation
    1,                  // copy number
    checkOverlaps);     // overlaps checking

  //===============================================================================
  // S2 detector

  // Geometry
  //  
  G4Tubs* pS2Geo = new G4Tubs(
    "S2Geo",
    11.53*mm,
    35.0*mm,
    64.0*um, // thickness
    0.0,
    2*CLHEP::pi);

  // logic definition
  //
  pS2Logical = new G4LogicalVolume(
    pS2Geo, // the geometry/solid 
    pSiMat,   // the material
    "S2Logic");	   // the name

  new G4PVPlacement(0,	//no rotation
    G4ThreeVector(0,0,71.0*mm),	  // the center at (0,0,0)
    pS2Logical,        // the logical volume
    "S2Logic",  // the name
    pWorldLogic,        // the mother (logical) volume
    false,              // no boolean operation
    1,                  // copy number
    checkOverlaps);     // overlaps checking


  //===============================================================================
  // C2D4 Target

  G4Material* C2D4 = new G4Material("C2D4", 1.06*g/cm3, 2);
  G4Element* C  = new G4Element("Carbon",   "C",  6.0,  12.011*g/mole);
  G4Isotope* D  = new G4Isotope("Deuteron", 1, 2, 2.0141018*g/mole);
  G4Element* elD = new G4Element("Deuterium", "elD", 1);
  elD->AddIsotope(D, 100.*perCent);
  C2D4->AddElement(C, 2);
  C2D4->AddElement(elD, 4);

  G4double targetThickness = 0.1*um; // 7.5
  G4VSolid* pTargetSolid = new G4Tubs("TargetSolid", 0.0, 10.0*mm, targetThickness/2.0, 0.0, 360.0*deg);
  pTargetLogical =  new G4LogicalVolume(pTargetSolid, C2D4, "targetLogical");
  new G4PVPlacement(
    0, 
    G4ThreeVector(0.0, 0.0, 0.0), 
    pTargetLogical, 
    "targetPhysical", 
    pWorldLogic, 
    false, 
    0, 
    checkOverlaps);

  // pTargetLogical->SetUserLimits(new G4UserLimits(0.02*targetThickness));
  // pStepLimit = new G4UserLimits(0.01*targetThickness);
  // pTargetLogical->SetUserLimits(pStepLimit);


  //===============================================================================
  // Ion chamber detector
  
  // Material
  //
  // G4Material* pICMat = nist->FindOrBuildMaterial("G4_BUTANE");
  double R = 62364.0; // cm3*Torr/(K mol)
  double molar_mass = 58.12; // g/mol
  double pressure = 124.0; // Torr
  double ic_rho = molar_mass * pressure / (R * 293.15);  // g/cm3
  G4Material* p_ic_gas_mat = nist->BuildMaterialWithNewDensity("butane","G4_BUTANE",ic_rho*g/cm/cm/cm);

  // Geometry inner gas
  //  
  G4Tubs* p_ic_gas_geo = new G4Tubs(
    "ic_gas_geo",
    0.0,
    10.95*cm,
    20.0*cm, // half z thickness
    0.0,
    2*CLHEP::pi);

  // logic definition
  //
  pICLogical = new G4LogicalVolume(
    p_ic_gas_geo, // the geometry/solid 
    p_ic_gas_mat,   // the material
    "ICLogic");	   // the name

  new G4PVPlacement(0,	//no rotation
    G4ThreeVector(0,0,(30.0+20.0)*cm), // the center
    pICLogical,        // the logical volume
    "ICLogic",  // the name
    pWorldLogic,        // the mother (logical) volume
    false,              // no boolean operation
    1,                  // copy number
    checkOverlaps);     // overlaps checking

  // DON'T STEP TOO LARGE
  pICLogical->SetUserLimits(new G4UserLimits(2.0*mm));

  // 
  G4VisAttributes* pICvis = new G4VisAttributes(
    G4Colour(
      0.0,   // red
      0.0,   // green
      1.0,   // blue
      0.5)); // alpha trans.
  pICLogical->SetVisAttributes(pICvis);


  // -----> Outer shell
  G4Material* pStainlessMat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4VisAttributes* pStainlessVis = new G4VisAttributes(
    G4Colour(
      0.75,   // red
      0.75,   // green
      0.75,   // blue
      0.5)); // alpha trans.

  G4Tubs* p_ic_cycl_geo = new G4Tubs(
    "ic_cycl_geo",
    0.0,
    11.93*cm,
    22*cm, // half z-thickness
    0.0,
    2*CLHEP::pi);
  
  G4Tubs* p_ic_front_hole = new G4Tubs(
    "ic_front_hole",
    0.0, 
    4.45*cm,// 6.35
    1.0*cm, // half z-thickness
    0.0,
    2*CLHEP::pi);

  G4VSolid* p_ic_shell = new G4SubtractionSolid("ic_shell", p_ic_cycl_geo, p_ic_gas_geo);
  G4VSolid* p_ic_geo = new G4SubtractionSolid("ic_geo", p_ic_shell, p_ic_front_hole, new G4RotationMatrix(), G4ThreeVector(0,0,-21.0*cm) );

  auto pICOuterLogical = new G4LogicalVolume(
    p_ic_geo, // the geometry/solid 
    pStainlessMat,   // the material
    "ICOuterLogical");	   // the name

  new G4PVPlacement(0,	//no rotation
    G4ThreeVector(0,0,(30.0+20.0)*cm), // the center
    pICOuterLogical,  // the logical volume
    "ICOuterLogical",   // the name
    pWorldLogic, // the mother (logical) volume
    false,       // no boolean operation
    1,      // copy number
    checkOverlaps); // overlaps checking

  pICOuterLogical->SetVisAttributes(pStainlessVis);

  // -----> Kapton Window
  G4Material* pKaptonMat = nist->FindOrBuildMaterial("G4_KAPTON");

  G4Tubs* p_ic_window_geo = new G4Tubs(
    "ic_window_geo",
    0,
    4.45*cm,
    3.5*um, // half z-thickness
    0.0,
    2*CLHEP::pi);

  auto p_ic_window_logical = new G4LogicalVolume(
    p_ic_window_geo, // the geometry/solid 
    pKaptonMat,   // the material
    "ic_window_logical");	   // the name

  new G4PVPlacement(0,	//no rotation
    G4ThreeVector(0,0,(30.0-3.5E-4)*cm), // the center
    p_ic_window_logical,  // the logical volume
    "ic_window_logical",   // the name
    pWorldLogic, // the mother (logical) volume
    false,       // no boolean operation
    1,      // copy number
    checkOverlaps); // overlaps checking
  

  // -----> beam stopper
  G4Tubs* p_ic_stopper_geo = new G4Tubs(
    "ic_stopper_geo",
    0,
    1.0*cm,
    0.5*cm, // thickness
    0.0,
    2*CLHEP::pi);

  auto p_ic_stopper_logical = new G4LogicalVolume(
    p_ic_stopper_geo, // the geometry/solid 
    pStainlessMat,   // the material
    "ic_stopper_logical");	   // the name

  new G4PVPlacement(0,	//no rotation
    G4ThreeVector(0,0,(30.0-2.0)*cm), // the center
    p_ic_stopper_logical,  // the logical volume
    "ic_stopper_logical",   // the name
    pWorldLogic, // the mother (logical) volume
    false,       // no boolean operation
    1,      // copy number
    checkOverlaps); // overlaps checking



  //===============================================================================
  // p-Terphenyl material from NIST
  //

  // Material Properties Table
  // for scintillation, Properties are expressed as a function of the photon’s momentum
  //
  G4Material* Terph = nist->FindOrBuildMaterial("G4_TERPHENYL");
  
  // // // Adding Scintillation property:
  // // std::vector<G4double> energy, scint, fast, slow, rindx, atten;
  // // {
  // //   // Read emission spectrum from data file
  // //   std::ifstream ifs("data/p-terphenyl_emission.dat");
  // //   if(!ifs.good()){
  // //     G4cerr << "ERROR: Couldn't open file: data/p-terphenyl_emission.dat" << G4endl;
  // //     exit(1);
  // //   }
  // //   G4double wl = 0.0, pr = 0.0, e = 0.0;
  // //   while(ifs >> wl >> pr) 
  // //   {
  // //     e = 1239.8 / wl; // nm to eV
    
  // //     energy.emplace_back(e * eV); // convert nm to energy
  // //     scint.emplace_back(pr);   // scintillation probability
  // //     rindx.emplace_back(1.65); // refractive index
  // //     fast.emplace_back(3.0 * ns);  // FAST component
  // //     slow.emplace_back(30.0 * ns); // SLOW component
  // //     atten.emplace_back(4.73 * mm); // Attenuation length (from https://arxiv.org/pdf/1305.0442.pdf)
  // //   }
  // // }
  
  // // Set Material Properties
  // G4MaterialPropertiesTable* scint_MPT = new G4MaterialPropertiesTable();
  // // scint_MPT->AddProperty("SCINTILLATIONCOMPONENT1", energy, scint);
  // // scint_MPT->AddProperty("SCINTILLATIONCOMPONENT2", energy, scint);
  // scint_MPT->AddConstProperty("SCINTILLATIONYIELD", 27000.0 / MeV); // from proteus
  // // scint_MPT->AddProperty("RINDEX", energy, rindx);
  // // scint_MPT->AddProperty("ABSLENGTH", energy, atten);
  // scint_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
  // scint_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3. * ns);
  // scint_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 40. * ns);
  // scint_MPT->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  // scint_MPT->AddConstProperty("SCINTILLATIONYIELD2", 0.2);

  // Terph->SetMaterialPropertiesTable(scint_MPT);
  // Terph->GetIonisation()->SetBirksConstant(0.126 * mm / MeV); // Set the Birks Constant for the scintillator


  // p-Terphenyl geometry
  //
  G4Box* p_neut_cube_geo = new G4Box("cube",2.5*cm,2.5*cm,2.5*cm);
  G4Tubs* p_neut_cylinder_geo = new G4Tubs(
    "cylinder",
    0.0*cm, // inner radius
    3.55*cm, // outer radius
    1.25*cm, // z-depth
    0.0, // start angle
    2*CLHEP::pi); // spanning angle

  // p-Terphenyl logic volume definition
  // 
  pScintLogical = new G4LogicalVolume(
    p_neut_cylinder_geo,  // the geometry/solid
    Terph,        // the material
    "Scintillator"); // the name
  // DON'T STEP TOO LARGE
  pScintLogical->SetUserLimits(new G4UserLimits(1.0*mm));

  // p-Terphenyl visual attributes, this can be skipped!
  //
  G4VisAttributes* scint_vis_att = new G4VisAttributes(
    G4Colour(0.0,   // red
             1.0,   // green
             1.0,   // blue
             0.6)); // alpha trans.
  pScintLogical->SetVisAttributes(scint_vis_att);

  // p-Terphenyl placement in the "World"
  //
  // positions in cm
  std::vector<double> scint_x = {
    -3.81, 3.81, 
    -11.43, -3.81, 3.81, 11.43,
    -15.06, -7.44, 7.44, 15.06,
    -11.43, -3.81, 3.81, 11.43,
    -3.81, 3.81
  }; // cm
  std::vector<double> scint_y = {
    15.24, 15.24,
    7.62, 7.62, 7.62, 7.62,
    0.0, 0.0, 0.0, 0.0,
    -7.62, -7.62, -7.62, -7.62,
    -15.24, -15.24
  };
  // std::vector<double> scint_z = {};

  for(int i = 0; i < scint_x.size(); i++) {

    new G4PVPlacement(0,	//no rotation
      G4ThreeVector(scint_x[i]*cm, scint_y[i]*cm, -23.0*cm),
      pScintLogical,        // the logical volume
      "Scintillator",     // the name
      pWorldLogic,        // the mother (logical) volume
      false,              // no boolean operation
      i,                  // copy number
      checkOverlaps);     // overlaps checking
  }


  //=================================================================================                   
  //always return the physical World
  //
  return pPhysWorld;   
}





//=================================================================================
// Construct for Sensitive Detector and Field

void DetectorConstruction::ConstructSDandField(){

  G4SDManager* pSDmanager = G4SDManager::GetSDMpointer();
  
  // Target
  auto pTarget = new GenericSD("Target");
  G4SDManager::GetSDMpointer()->AddNewDetector(pTarget);
  pTargetLogical->SetSensitiveDetector(pTarget);

  // S1-Detector
  auto pS1 = new GenericSD("S1");
  G4SDManager::GetSDMpointer()->AddNewDetector(pS1);
  pS1Logical->SetSensitiveDetector(pS1);
  
  // S2-Detector
  auto pS2 = new GenericSD("S2");
  G4SDManager::GetSDMpointer()->AddNewDetector(pS2);
  pS2Logical->SetSensitiveDetector(pS2);

  // IC-Detector
  auto pIC = new GenericSD("IC");
  G4SDManager::GetSDMpointer()->AddNewDetector(pIC);
  pICLogical->SetSensitiveDetector(pIC);

  // Scint-Detector
  auto pScint = new GenericSD("Scint");
  G4SDManager::GetSDMpointer()->AddNewDetector(pScint);
  pScintLogical->SetSensitiveDetector(pScint);
}
