#include "DetectorGeometry.hh"

DetectorGeometry::DetectorGeometry()
{
   fAl = fAir = fGlass = fLS = nullptr; 
   fN = fO = fC = fH = nullptr;

   DefineMaterials();
}

DetectorGeometry::~DetectorGeometry()
{
  delete MatTab_Air;
  delete MatTab_Al;
  delete MatTab_LS;
  delete MatTab_PMMA;
  delete MatTab_Glass;
}

void DetectorGeometry::DefineMaterials()
{
    //Load all materials:
    Load_Elements();
    Load_Material_Air();
    Load_Material_PMMA();
    Load_Material_Aluminium();
    Load_Material_FusedSilica();
    //Load_Material_SlowLS(17.68,68.88,310.12);
    Load_Material_WbLS(2.56,17.56);
}

G4VPhysicalVolume *DetectorGeometry::Construct()
{
    /*************************World Volume*********************************/
    G4Box *solidWorld = new G4Box("solidWorld",1.0*m,1.0*m,1.0*m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, fAir, "logicWorld");
    G4VPhysicalVolume *physicalWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physicalWorld", 0, false, 0, true);

    /*************************Cell Qube*********************************/
    G4double length = 350.;
    G4double wallThickness = 5.;
    G4Box *outerBox = new G4Box("outerBox",(length+2*wallThickness)/2*mm,(length+2*wallThickness)/2*mm,(length+2*wallThickness)/2*mm);
    G4Box *innerBox = new G4Box("innerBox",(length)/2*mm,(length)/2*mm,(length)/2*mm);
    G4SubtractionSolid *hollowCell = new G4SubtractionSolid("hollowCell",outerBox,innerBox);
    G4LogicalVolume *logicCell= new G4LogicalVolume(hollowCell, fAl, "logicCell");
    G4VPhysicalVolume* physicalCell = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicCell, "physicalCell", logicWorld, false, 0, true);

    /*************************Cell air volume*********************************/
    G4Box *solidAir = new G4Box("solidAir",(length)/2*mm,(length)/2*mm,(length)/2*mm);
    G4LogicalVolume *logicAir = new G4LogicalVolume(solidAir, fAir, "logicAir");
    G4VPhysicalVolume* physicalAir = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicAir, "physicalAir", logicWorld, false, 0, true);

    /*************************Liquid scintillator vessel*********************************/
    G4RotationMatrix* sphereRotation = new G4RotationMatrix();
    sphereRotation->rotateX(90.*deg);
    sphereRotation->rotateY(0.*deg);
    sphereRotation->rotateZ(0.*rad);
    G4double position = -70.;
    G4double shift = 0.;

    G4double radiusLiquid = 65/2;
    G4double acrylicThickness = 1.;
    G4double radiusAcryl = radiusLiquid + acrylicThickness;
    G4Sphere *AcrylSphere = new G4Sphere("AcrylSphere", radiusLiquid*mm, radiusAcryl*mm, 0.*deg, 360.*deg, 0.*deg, 360.*deg);
    G4LogicalVolume *logicLsVessel= new G4LogicalVolume(AcrylSphere, fGlass, "logicLsVessel");
    G4VPhysicalVolume* physicalVessel = new G4PVPlacement(sphereRotation, G4ThreeVector(shift*mm,0.,position*mm), logicLsVessel, "physicalLsVessel", logicAir, false, 0, true);

    /*************************Liquid scintillator*********************************/
    G4Sphere *LsSphere = new G4Sphere("LsSphere", 0.*mm, radiusLiquid*mm, 0.*deg, 360.*deg, 0.*deg, 360.*deg);
    G4LogicalVolume *logicLS= new G4LogicalVolume(LsSphere, fLS, "logicLS");
    G4VPhysicalVolume* physicalLS = new G4PVPlacement(sphereRotation, G4ThreeVector(shift*mm,0.,position*mm), logicLS, "physicalLS", logicAir, false, 0, true);

    /*************************Back plane*****************************************/
    //G4Tubs *backPlane = new G4Tubs("backPlane", 0.*mm, radiusAcryl*mm, wallThickness/2*mm, 0.*deg, 360.*deg);
    //G4LogicalVolume *logicbackPlane = new G4LogicalVolume(backPlane, fAl, "logicbackPlane");
    //G4VPhysicalVolume* physicalbackPlane = new G4PVPlacement(0, G4ThreeVector(shift*mm,0.,(position+wallThickness/2)*mm), logicbackPlane, "physicalbackPlane", logicAir, false, 0, true);



    /*************************Detector Geometry*********************************/
    G4double d_PMT1i = 33.;
    G4double d_PMT2i = 52.;
    G4double PMT_thickness = 3.;
    G4double POS_side_x = radiusAcryl + 5.;
    G4double POS_side_y = 0.;
    G4double POS_side_z = position;
    G4double POS_bot_x[5] = {0., 0., 0., -67.642, 67.642};
    G4double POS_bot_y[5] = {0., -67.642, 67.642, 0., 0.}; 
    G4double POS_bot_z = (length/2-2*wallThickness);
    G4RotationMatrix* PMTRotation = new G4RotationMatrix();
    PMTRotation->rotateX(0.*deg);
    PMTRotation->rotateY(90.*deg);
    PMTRotation->rotateZ(0.*rad);

    G4Tubs *PMT1i = new G4Tubs("PMT1i", 0.*mm, d_PMT1i/2*mm, PMT_thickness*mm, 0.*deg, 360.*deg);
    logicPMT1i = new G4LogicalVolume(PMT1i, fGlass, "logicPMT1i");

    G4Tubs *PMT2i = new G4Tubs("PMT2i", 0.*mm, d_PMT2i/2*mm, PMT_thickness*mm, 0.*deg, 360.*deg);
    logicPMT2i = new G4LogicalVolume(PMT2i, fGlass, "logicPMT2i");

    
    for(G4int i=1; i<7; i++)
    {
       if(i==0)
       {
         G4VPhysicalVolume *physicalPMT = new G4PVPlacement(0,G4ThreeVector(POS_bot_x[i]*mm,POS_bot_y[i]*mm,POS_bot_z*mm), logicPMT2i,"physicalPMT",logicAir,false,i,true);
       }else if(i<5 && i!=0)
       {
         G4VPhysicalVolume *physicalPMT = new G4PVPlacement(0,G4ThreeVector(POS_bot_x[i]*mm,POS_bot_y[i]*mm,POS_bot_z*mm), logicPMT1i,"physicalPMT",logicAir,false,i,true);
       }else
       {
          G4VPhysicalVolume *physicalPMT = new G4PVPlacement(PMTRotation,G4ThreeVector(POS_side_x*mm,pow(-1,i)*POS_side_y*mm,POS_side_z*mm), logicPMT1i,"physicalPMT",logicAir,false,i,true);
       }
    }


    /*************************Optical surfaces*********************************/
    /*************************Liquid scintillator*********************************/
    G4OpticalSurface* OpLsSurface = new G4OpticalSurface("OpLsSurface");
    OpLsSurface->SetType(dielectric_dielectric);
    OpLsSurface->SetFinish(ground);
    OpLsSurface->SetModel(glisur);

    /*************************Air*********************************/
    G4OpticalSurface* OpAirSurface = new G4OpticalSurface("OpAirSurface");
    OpAirSurface->SetType(dielectric_dielectric);
    OpAirSurface->SetFinish(ground);
    OpAirSurface->SetModel(glisur);

    /*************************Aluminium*********************************/
    G4OpticalSurface* OpAluSurface = new G4OpticalSurface("OpAluSurface");
    OpAluSurface->SetType(dielectric_metal);
    OpAluSurface->SetFinish(ground);
    OpAluSurface->SetModel(glisur);

    /*************************Plastic*********************************/
    G4OpticalSurface* OpPsSurface = new G4OpticalSurface("OpPsSurface");
    OpPsSurface->SetType(dielectric_dielectric);
    OpPsSurface->SetFinish(ground);
    OpPsSurface->SetModel(glisur);


    G4LogicalBorderSurface* Cell_AlAir = new G4LogicalBorderSurface("Cell_AlAir", physicalCell, physicalAir, OpAirSurface);
    G4OpticalSurface* opticalSurface = dynamic_cast<G4OpticalSurface*>(Cell_AlAir->GetSurface(physicalCell, physicalAir)->GetSurfaceProperty());
    if(opticalSurface){opticalSurface->DumpInfo();}

    G4LogicalBorderSurface* Vessel_AirPs = new G4LogicalBorderSurface("Vessel_AirPs", physicalAir, physicalVessel, OpAluSurface);
    opticalSurface = dynamic_cast<G4OpticalSurface*>(Vessel_AirPs->GetSurface(physicalAir, physicalVessel)->GetSurfaceProperty());
    if(opticalSurface){opticalSurface->DumpInfo();}

    G4LogicalBorderSurface* Vessel_PsLs = new G4LogicalBorderSurface("Vessel_PsLs", physicalVessel, physicalLS, OpLsSurface);
    opticalSurface = dynamic_cast<G4OpticalSurface*>(Vessel_PsLs->GetSurface(physicalVessel, physicalLS)->GetSurfaceProperty());
    if(opticalSurface){opticalSurface->DumpInfo();}

    G4LogicalBorderSurface* Vessel_LsPs = new G4LogicalBorderSurface("Vessel_LsPs", physicalLS, physicalVessel, OpPsSurface);
    opticalSurface = dynamic_cast<G4OpticalSurface*>(Vessel_LsPs->GetSurface(physicalLS, physicalVessel)->GetSurfaceProperty());
    if(opticalSurface){opticalSurface->DumpInfo();}

    G4LogicalBorderSurface* Vessel_PsAir = new G4LogicalBorderSurface("Vessel_PsAir", physicalVessel, physicalAir, OpAirSurface);
    opticalSurface = dynamic_cast<G4OpticalSurface*>(Vessel_PsAir->GetSurface(physicalVessel, physicalAir)->GetSurfaceProperty());
    if(opticalSurface){opticalSurface->DumpInfo();}

    G4LogicalBorderSurface* Vessel_AlLs = new G4LogicalBorderSurface("Vessel_AlLs", physicalCell, physicalLS, OpAirSurface);
    opticalSurface = dynamic_cast<G4OpticalSurface*>(Vessel_AlLs->GetSurface(physicalCell, physicalLS)->GetSurfaceProperty());
    if(opticalSurface){opticalSurface->DumpInfo();}

    return physicalWorld;

}

void DetectorGeometry::ConstructSDandField()
{
    SipmArray *SArr = new SipmArray("DetectorArray");
    logicPMT1i->SetSensitiveDetector(SArr);
    logicPMT2i->SetSensitiveDetector(SArr);
}

void DetectorGeometry::Load_Elements()
{
    /*************************Elements*********************************/
    fH = new G4Element("H", "H", z = 1., a = 1.01 * g / mole);
    fC = new G4Element("C", "C", z = 6., a = 12.01 * g / mole);
    fN = new G4Element("N", "N", z = 7., a = 14.01 * g / mole);
    fO = new G4Element("O", "O", z = 8., a = 16.00 * g / mole);
    fSi = new G4Element("Si", "Si", z=14., a= 28.09* g/mole);
}

void DetectorGeometry::Load_Material_PMMA()
{
    /*************************PMMA*********************************/
    fPMMA = new G4Material("PMMA", density = 1.190*g/cm3, 3);
    fPMMA->AddElement(fC, 5);
    fPMMA->AddElement(fH, 8);
    fPMMA->AddElement(fO, 2);
   
    const G4int NENTRIES = 11 ;
    G4double lambda_max = 700*nm;
    G4double lambda_min = 200*nm;
    G4double LAMBDA_PMMA[NENTRIES];
    G4double RINDEX_PMMA[NENTRIES];
    G4double ENERGY_PMMA[NENTRIES];
   
    G4double bParam[4] = {1760.7010,-1.3687,2.4388e-3,-1.5178e-6} ; 
    for(G4int i=0;i<NENTRIES; i++)
    {
        LAMBDA_PMMA[i] = lambda_min + i*(lambda_max-lambda_min)/float(NENTRIES-1) ;
        RINDEX_PMMA[i] = 0.0 ;
        for (G4int jj=0 ; jj<4 ; jj++)
        {
           RINDEX_PMMA[i] +=  (bParam[jj]/1000.0)*std::pow(LAMBDA_PMMA[i]/nm,jj) ; 
        }
        ENERGY_PMMA[i] =   h_Planck*c_light/LAMBDA_PMMA[i] ;  // Convert from wavelength to energy ;
        //G4cout << ENERGY_PMMA[i]/eV << " " << LAMBDA_PMMA[i]/nm << " " << RINDEX_PMMA[i] << G4endl ;
    }
    MatTab_PMMA = new G4MaterialPropertiesTable();
    MatTab_PMMA->AddProperty("RINDEX", ENERGY_PMMA, RINDEX_PMMA, NENTRIES);
   
    //Absorption
    const G4int NENT = 25 ;
    G4double LAMBDAABS[NENT] = 
    {
       100.0,
       246.528671, 260.605103, 263.853516, 266.019104, 268.726105,    
       271.433136, 273.598724, 276.305725, 279.554138, 300.127380,    
       320.159241, 340.191101, 360.764343, 381.337585, 399.745239,    
       421.401276, 440.891724, 460.382172, 480.414001, 500.987274,    
       520.477722, 540.509583, 559.458618,
       700.0    
    };
   
    //Wls absorption
    G4double ABSORPTION[NENT] =
    {
       0.*mm,
       0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm,  
       0.*mm, 0.*mm, 0.*mm, 0.*mm, 0.*mm,
       2.*cm, 5.*cm, 4.*cm, 1.*cm, 10.*cm,
       10.*m, 10.*m, 1.*km, 1.*km, 1.*km,
       1.*km, 1.*km, 1.*km,
       1.*km
    };

    //Re-emission
    G4double EMISSION[NENT] =
    {
       0.,
       0., 0., 0., 0., 0.,  
       0., 0., 0., 0., 0.,
       0., 0., 0., 0.05, 0.9,
       0.99, 0.59, 0.4, 0.2, 0.09,
       0., 0., 0.,
       0.
    };

    G4double ABS[NENT] =   // Transmission (in %) of  3mm thick PMMA 
    { 
       0.0000000,
       0.0000000,  5.295952,  9.657321, 19.937695, 29.283491, 
       39.252335, 48.598133, 58.255451, 65.109039, 79.439247,
       85.669785, 89.719627, 91.277260, 91.588783, 91.900307,
       91.588783, 91.277260, 91.277260, 91.588783, 91.588783,
       91.900307, 91.900307, 91.588783,
       91.5
    };

    G4double WLSENERGY[NENT];
    for(G4int i=0;i<NENT; i++)
    {
       WLSENERGY[i] = h_Planck*c_light/(LAMBDAABS[i]*nm) ;
    }

    MatTab_PMMA->AddProperty("ABSLENGTH", new G4MaterialPropertyVector()) ;
    for(G4int i=0;i<NENT; i++)
    {
       G4double energy = h_Planck*c_light/(LAMBDAABS[i]*nm) ;
       G4double abslength ;
   
       if (ABS[i] <= 0.0) {
         abslength = 1.0/kInfinity ;
       }
       else {
         abslength = -3.0*mm/(std::log(ABS[i]/100.0)) ;
       }
       MatTab_PMMA->AddEntry("ABSLENGTH", energy, abslength);
    }
    //MatTab_PMMA->DumpTable();
    MatTab_PMMA->AddProperty("WLSABSLENGTH", WLSENERGY, ABSORPTION, NENT);
    MatTab_PMMA->AddProperty("WLSCOMPONENT", WLSENERGY, EMISSION, NENT);
    MatTab_PMMA->AddConstProperty("WLSTIMECONSTANT", 2.5*ns);
    fPMMA->SetMaterialPropertiesTable(MatTab_PMMA);
}

void DetectorGeometry::Load_Material_Air()
{
    /*************************Air*********************************/
    fAir = new G4Material("Air", density = 1.29 * mg / cm3, 2);
    fAir->AddElement(fN, 70 * perCent);
    fAir->AddElement(fO, 30 * perCent);
    MatTab_Air = new G4MaterialPropertiesTable();
    G4double Eair[2] = {0.1*eV, 100*eV};
    G4double Rair[2] = {1., 1.};
    MatTab_Air->AddProperty("RINDEX", Eair, Rair, 2);
    //MatTab_Air->DumpTable();
    fAir->SetMaterialPropertiesTable(MatTab_Air);

}

void DetectorGeometry::Load_Material_SlowLS(G4double Tau1, G4double Tau2, G4double Tau3)
{
    /*************************Ĺiquid scintillator*********************************/
    fLS = new G4Material("LS", density = 860. * kg / m3, 2);
    fLS->AddElement(fH, 30);
    fLS->AddElement(fC, 18);

    const G4int NENTRIES_LS = 21;
    G4double LAMBDA_LS[NENTRIES_LS];
    G4double ENERGY_LS[NENTRIES_LS];
    G4double RINDEX_LS[NENTRIES_LS];

    G4double lambda_max = 700*nm;
    G4double lambda_min = 200*nm;


    for(G4int i=0;i<NENTRIES_LS; i++)
    {
        LAMBDA_LS[i] = lambda_min + i*(lambda_max-lambda_min)/float(NENTRIES_LS-1) ;
        ENERGY_LS[i] = h_Planck*c_light/LAMBDA_LS[i] ; 
        RINDEX_LS[i] = 1.5;
    }

    G4double ABSLEN_LS[NENTRIES_LS] =  
    { 
        0.01*mm,
        0.01*mm,    0.01*mm,    0.01*mm,    0.01*mm,    0.05*mm,
        1.*mm,      10.*m,      50.*m,      100.*m,     100.*m,
        100.*m,     100.*m,     100.*m,     100.*m,     100.*m,
        100.*m,     100.*m,     100.*m,     100.*m,     100.*m,
    };

    G4double SCINT_LS[NENTRIES_LS] =
    {
        0., 
        0., 0., 0., 0., 0.01,
        0.4, 0.6, 0.3, 0.1, 0.05,
        0.02, 0.01, 0., 0., 0.,
        0., 0., 0., 0., 0.
    };

    MatTab_LS = new G4MaterialPropertiesTable();
    MatTab_LS->AddProperty("SCINTILLATIONCOMPONENT1", ENERGY_LS, SCINT_LS, NENTRIES_LS);
    MatTab_LS->AddProperty("SCINTILLATIONCOMPONENT2", ENERGY_LS, SCINT_LS, NENTRIES_LS);
    MatTab_LS->AddProperty("RINDEX", ENERGY_LS, RINDEX_LS, NENTRIES_LS);
    MatTab_LS->AddProperty("ABSLENGTH", ENERGY_LS, ABSLEN_LS, NENTRIES_LS);
    MatTab_LS->AddConstProperty("SCINTILLATIONYIELD", 7500/ MeV);
    MatTab_LS->AddConstProperty("RESOLUTIONSCALE", 1.0);
    MatTab_LS->AddConstProperty("SCINTILLATIONTIMECONSTANT1", Tau1* ns);  //17 --- 2
    MatTab_LS->AddConstProperty("SCINTILLATIONTIMECONSTANT2", Tau2* ns); //68 --- 25
    MatTab_LS->AddConstProperty("SCINTILLATIONTIMECONSTANT3", Tau3* ns); //68 --- 25
    MatTab_LS->AddConstProperty("SCINTILLATIONYIELD1", 0.8274);
    MatTab_LS->AddConstProperty("SCINTILLATIONYIELD2", 0.1118);
    MatTab_LS->AddConstProperty("SCINTILLATIONYIELD3", 0.0608);
    //MatTab_LS->DumpTable();
    fLS->SetMaterialPropertiesTable(MatTab_LS);
}

void DetectorGeometry::Load_Material_WbLS(G4double Tau1, G4double Tau2)
{
    /*************************Ĺiquid scintillator*********************************/
    fLS = new G4Material("LS", density = 997.* kg/m3, 2);
    fLS->AddElement(fH, 2);
    fLS->AddElement(fO, 1);

    G4double lambda_max = 700*nm;
    G4double lambda_min = 200*nm;

    const G4int NENTRIES_LS = 21;
    G4double LAMBDA_LS[NENTRIES_LS];
    G4double ENERGY_LS[NENTRIES_LS];
    G4double RINDEX_LS[NENTRIES_LS];

    for(G4int i=0;i<NENTRIES_LS; i++)
    {
        LAMBDA_LS[i] = lambda_min + i*(lambda_max-lambda_min)/float(NENTRIES_LS-1) ;
        ENERGY_LS[i] = h_Planck*c_light/LAMBDA_LS[i] ; 
        RINDEX_LS[i] = 1.33;
    }

    G4double ABSLEN_LS[NENTRIES_LS] =  
    { 
        0.01*mm,
        0.01*mm,    0.01*mm,    0.01*mm,    0.01*mm,    0.05*mm,
        1.*mm,      10.*m,      50.*m,      100.*m,     100.*m,
        100.*m,     100.*m,     100.*m,     100.*m,     100.*m,
        100.*m,     100.*m,     100.*m,     100.*m,     100.*m,
    };

    G4double SCINT_LS[NENTRIES_LS] =
    {
        0., 
        0., 0., 0., 0., 0.01,
        0.4, 0.6, 0.3, 0.1, 0.05,
        0.02, 0.01, 0., 0., 0.,
        0., 0., 0., 0., 0.
    };

    MatTab_LS = new G4MaterialPropertiesTable();
    MatTab_LS->AddProperty("SCINTILLATIONCOMPONENT1", ENERGY_LS, SCINT_LS, NENTRIES_LS);
    MatTab_LS->AddProperty("SCINTILLATIONCOMPONENT2", ENERGY_LS, SCINT_LS, NENTRIES_LS);
    MatTab_LS->AddProperty("RINDEX", ENERGY_LS, RINDEX_LS, NENTRIES_LS);
    MatTab_LS->AddProperty("ABSLENGTH", ENERGY_LS, ABSLEN_LS, NENTRIES_LS);
    MatTab_LS->AddConstProperty("SCINTILLATIONYIELD", 750 / MeV);
    MatTab_LS->AddConstProperty("RESOLUTIONSCALE", 1.0);
    MatTab_LS->AddConstProperty("SCINTILLATIONTIMECONSTANT1", Tau1* ns);  //17 --- 2
    MatTab_LS->AddConstProperty("SCINTILLATIONTIMECONSTANT2", Tau2* ns); //68 --- 25
    MatTab_LS->AddConstProperty("SCINTILLATIONYIELD1", 0.8274);
    MatTab_LS->AddConstProperty("SCINTILLATIONYIELD2", 0.1118);
    //MatTab_LS->DumpTable();
    fLS->SetMaterialPropertiesTable(MatTab_LS);
}

void DetectorGeometry::Load_Material_FusedSilica()
{ 
    density = 2.200*g/cm3; // fused quartz 
  
    fGlass = new G4Material("Quartz",density, 2);
    fGlass->AddElement(fSi, 1) ;
    fGlass->AddElement(fO , 2) ;

    G4double lambda_max = 700*nm;
    G4double lambda_min = 200*nm;


    const G4int N_RINDEX_QUARTZ = 2 ;
    G4double X_RINDEX_QUARTZ[N_RINDEX_QUARTZ] = {h_Planck*c_light/lambda_max, h_Planck*c_light/lambda_min} ; 
    G4double RINDEX_QUARTZ[N_RINDEX_QUARTZ] = {1.54, 1.54};

    MatTab_Glass = new G4MaterialPropertiesTable();
    MatTab_Glass->AddProperty("RINDEX", X_RINDEX_QUARTZ, RINDEX_QUARTZ, N_RINDEX_QUARTZ);
    fGlass->SetMaterialPropertiesTable(MatTab_Glass);
}

void DetectorGeometry::Load_Material_Aluminium()
{
    /*************************Aluminium*********************************/
    fAl = new G4Material("Al", z = 13., a = 26.98 * g / mole, density = 2.7 * g / cm3);
    MatTab_Al = new G4MaterialPropertiesTable();
    //MatTab_Al->DumpTable();
    fAl->SetMaterialPropertiesTable(MatTab_Al);
}
