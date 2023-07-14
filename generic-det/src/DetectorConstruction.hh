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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "globals.hh"
#include <vector>

// User functions
//
#include "ScintSD.hh"
#include "G4SDManager.hh"

// Detector construction class to define materials and geometry.
//
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    virtual void ConstructSDandField();
    // G4LogicalVolume* GetWorldVolume() const {return scint_logic;}
    G4LogicalVolume* GetWorldVolume() const {return world_logic;}
    G4LogicalVolume* GetTargetVolume() const {return TargetLogical;}
    G4LogicalVolume* GetDetectVolume() const {return DetectLogical;}

  private:
    G4LogicalVolume* scint_logic;
    G4LogicalVolume* world_logic;
    G4LogicalVolume* TargetLogical;
    G4LogicalVolume* DetectLogical;

};


#endif

