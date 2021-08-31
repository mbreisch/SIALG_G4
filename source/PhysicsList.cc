#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{
	RegisterPhysics (new G4EmStandardPhysics());

	G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
	auto opticalParams               = G4OpticalParameters::Instance();

	opticalParams->SetWLSTimeProfile("delta");

	opticalParams->SetScintYieldFactor(1.0);
	opticalParams->SetScintExcitationRatio(0.0);
	opticalParams->SetScintTrackSecondariesFirst(true);
	opticalParams->SetScintEnhancedTimeConstants(true);

	opticalParams->SetCerenkovMaxPhotonsPerStep(100);
	opticalParams->SetCerenkovMaxBetaChange(10.0);
	opticalParams->SetCerenkovTrackSecondariesFirst(true);

	RegisterPhysics (opticalPhysics);
}

PhysicsList::~PhysicsList()
{}