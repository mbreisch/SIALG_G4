#ifndef DetectorGeometry_HH
#define DetectorGeometry_HH

#include <cmath>
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"
#include "G4Cons.hh"
#include "G4RotationMatrix.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4NistManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4Cache.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolumeStore.hh"

#include "SipmArray.hh"

class DetectorGeometry :  public G4VUserDetectorConstruction
{
    public:
        DetectorGeometry();
        ~DetectorGeometry();

        virtual G4VPhysicalVolume *Construct();
    private:

        G4LogicalVolume *logicSiPM;
        G4LogicalVolume *logicLGcone;
        G4LogicalVolume *logicLGell;
        G4LogicalVolume *logicPMT1i;
        G4LogicalVolume *logicPMT2i;
        virtual void ConstructSDandField();

        G4double a;  // atomic mass
        G4double z;  // atomic number
        G4double density;


        void DefineMaterials();
        void Load_Elements();
        void Load_Material_Air();
        void Load_Material_PMMA();
        void Load_Material_Aluminium();
        void Load_Material_FusedSilica();
        void Load_Material_SlowLS(G4double Tau1, G4double Tau2, G4double Tau3);
        void Load_Material_WbLS(G4double Tau1, G4double Tau2);
        G4Material* fPMMA;
        G4Material* fAir;
        G4Material* fLS;
        G4Material* fGlass;
        G4Material* fAl;
        G4Element* fN;
        G4Element* fO;
        G4Element* fC;
        G4Element* fH;
        G4Element* fSi;

        G4MaterialPropertiesTable* MatTab_Air;
        G4MaterialPropertiesTable* MatTab_Al;
        G4MaterialPropertiesTable* MatTab_LS;
        G4MaterialPropertiesTable* MatTab_PMMA;
        G4MaterialPropertiesTable* MatTab_Glass;
};

#endif