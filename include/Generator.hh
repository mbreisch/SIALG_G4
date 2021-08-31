#ifndef Generator_HH
#define Generator_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalConstants.hh"

class Generator : public G4VUserPrimaryGeneratorAction
{
	public:
		Generator();
		~Generator();

		virtual void GeneratePrimaries(G4Event*);

	private:
		G4double size;
		G4Tubs* fVolume;
		G4ParticleGun *fParticleGun;

};


#endif