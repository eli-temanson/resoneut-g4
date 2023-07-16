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
// #include "ScintSD.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
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
  G4Box* pSolidWorld = new G4Box("World",1*m,1*m,1*m);

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
  // S2 detector
  
  // Material
  //
  G4Material* pS1Mat = nist->FindOrBuildMaterial("G4_Si");

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
  pDetectLogical = new G4LogicalVolume(
    pS1Geo, // the geometry/solid 
    pS1Mat,   // the material
    "DetectLogic");	   // the name

  new G4PVPlacement(0,	//no rotation
    G4ThreeVector(0,0,120.0*mm),	  // the center at (0,0,0)
    pDetectLogical,        // the logical volume
    "DetectLogic",  // the name
    pWorldLogic,        // the mother (logical) volume
    false,              // no boolean operation
    1,                  // copy number
    checkOverlaps);     // overlaps checking

  // //===============================================================================
  // // C2D4 Target
  // // 6.60 um for 0.7 mg/cm2
  // // 3.77 um for 0.4 mg/cm2
  // G4Material* C2D4 = new G4Material("C2D4", 1.06*g/cm3, 2);
  // G4Element* C  = new G4Element("Carbon",   "C",  6.0,  12.011*g/mole);
  // G4Isotope* D  = new G4Isotope("Deuteron", 1, 2, 2.0141018*g/mole);
  // G4Element* elD = new G4Element("Deuterium", "elD", 1);
  // elD->AddIsotope(D, 100.*perCent);
  // C2D4->AddElement(C, 2);
  // C2D4->AddElement(elD, 4);

  // G4double targetThickness = 6.60*um;
  // G4VSolid* pTargetSolid = new G4Tubs("TargetSolid", 0.0, 20.0*mm, targetThickness/2.0, 0.0, 360.0*deg);
  // TargetLogical =  new G4LogicalVolume(pTargetSolid, C2D4, "targetLogical");
  // new G4PVPlacement(0, 
  //                   G4ThreeVector(0.0, 0.0, 0.0), 
  //                   pTargetLogical, 
  //                   "targetPhysical", 
  //                   pWorldLogic, 
  //                   false, 
  //                   0, 
  //                   checkOverlaps);

  // pStepLimit = new G4UserLimits(0.02*targetThickness);
  // TargetLogical->SetUserLimits(pStepLimit);

  // //===============================================================================
  // // p-Terphenyl material from NIST
  // //

  // // Material Properties Table
  // // for scintillation, Properties are expressed as a function of the photon’s momentum
  // //
  // G4Material* Terph = nist->FindOrBuildMaterial("G4_TERPHENYL");
  
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

  // // p-Terphenyl geometry
  // //

  // // G4Box* cube = new G4Box("cube",2.5*cm,2.5*cm,2.5*cm);
  // G4Tubs* cylinder = new G4Tubs("cylinder",
  //                             0.0*cm, // inner radius
  //                             3.5*cm, // outer radius
  //                             2.5*cm, // z-depth
  //                             0.0*deg, // start angle
  //                             360.0*deg); // spanning angle

  // // p-Terphenyl logic volume definition
  // // 
  // scint_logic = new G4LogicalVolume(cylinder,  // the geometry/solid
  //                                   Terph,        // the material
  //                                   "Scintillator"); // the name

  // // p-Terphenyl visual attributes, this can be skipped!
  // //
  // G4VisAttributes* scint_vis_att = new G4VisAttributes(
  //   G4Colour(0.0,   // red
  //            1.0,   // green
  //            1.0,   // blue
  //            0.6)); // alpha trans.
  // scint_logic->SetVisAttributes(scint_vis_att);

  // // p-Terphenyl placement in the "World"
  // //
  // // G4VPhysicalVolume* scint_phys = // not needed with sensitive detector
  // new G4PVPlacement(0,	//no rotation
  //   G4ThreeVector(0,0,-5*cm),	  // the center at (0,0,0)
  //   scint_logic,        // the logical volume
  //   "Scintillator",     // the name
  //   pWorldLogic,        // the mother (logical) volume
  //   false,              // no boolean operation
  //   1,                  // copy number
  //   checkOverlaps);     // overlaps checking


  // //=================================================================================                   
  // //
  // // Tantalum
  // //
  // nist->FindOrBuildMaterial("G4_Ta");
  // auto Tantalum = G4Material::GetMaterial("G4_Ta");

  // G4Box* small_plate = new G4Box("small_plate",2.5*cm,2.5*cm,0.25*mm);

  // new G4LogicalVolume(
  //   small_plate,  // the geometry/solid
  //   Tantalum,        // the material
  //   "Ta_shield"); // the name

  // auto TaLV = new G4LogicalVolume(
  //   small_plate,        // its solid
  //   Tantalum, // its material
  //   "TaLV");        // its name

  // // comment this out to remove the tantalum
  // new G4PVPlacement(
  //   0,                // no rotation
  //   G4ThreeVector(0.0, 0.0, -9.0*cm), // its position
  //   TaLV,       // its logical volume
  //   "Ta",           // its name
  //   pWorldLogic,          // its mother  volume
  //   false,            // no boolean operation
  //   0,                // copy number
  //   checkOverlaps);  // checking overlaps



  //=================================================================================                   
  //always return the physical World
  //
  return pPhysWorld;   
}





//=================================================================================
// Construct for Sensitive Detector and Field

void DetectorConstruction::ConstructSDandField(){

  // auto scint_sensitive = new ScintSD("scint");
  // G4SDManager::GetSDMpointer()->AddNewDetector(scint_sensitive);
  // if(scint_logic != NULL) scint_logic->SetSensitiveDetector(scint_sensitive);

  G4SDManager* pSDmanager = G4SDManager::GetSDMpointer();

  // Silicon
  // G4VSensitiveDetector* pS1 = new GenericSD("S1");
  auto pS1 = new GenericSD("S1");
  G4SDManager::GetSDMpointer()->AddNewDetector(pS1);
  // pSDmanager->AddNewDetector(pS1);
  pDetectLogical->SetSensitiveDetector(pS1);

}
