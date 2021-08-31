#include "Generator.hh"

Generator::Generator()
{
	fParticleGun = new G4ParticleGun(1);
}

Generator::~Generator()
{
	delete fParticleGun;
}

void Generator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName = "mu-";
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

	/*
    G4LogicalVolume* aimVol = G4LogicalVolumeStore::GetInstance()->GetVolume("logicbackPlane");
    if(aimVol)
    {
    	fVolume = dynamic_cast<G4Tubs*>(aimVol->GetSolid());
    }
    if(fVolume) 
    {
    	size = fVolume->GetOuterRadius();
  	} 

  	G4double r = sqrt(G4UniformRand())*size;
  	G4double phi = 2*pi*G4UniformRand();

	G4double x0 = r * cos(phi);
 	G4double y0 = r * sin(phi);
	*/
	G4ThreeVector pos(0.*mm,0.*mm,1.0*m);
	G4ThreeVector mom(0.,0.,-1.0);

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(500.*MeV);
	fParticleGun->SetParticleDefinition(particle);

	fParticleGun->GeneratePrimaryVertex(anEvent);

}