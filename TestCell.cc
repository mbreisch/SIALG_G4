//C++ include
#include <iostream>

//Geant4 include
#include "G4RunManagerFactory.hh"
#include "G4String.hh"
#include "G4Types.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//Detector geometry include
#include "DetectorGeometry.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
	// detect interactive mode (if no arguments) and define UI session
	G4UIExecutive* ui = nullptr;
	if(argc == 1)
	{
		ui = new G4UIExecutive(argc, argv);
	}

	//Basic Geant4 Managers
	auto runManager = G4RunManagerFactory::CreateRunManager();
	
	
	runManager->SetUserInitialization(new DetectorGeometry());
	runManager->SetUserInitialization(new PhysicsList());
	runManager->SetUserInitialization(new ActionInitialization());

	runManager->Initialize();

	// initialize visualization
	G4VisManager* visManager = new G4VisExecutive();
	visManager->Initialize();

	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if(ui)
	{
		// get the pointer to the User Interface manager
		UImanager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
	}else
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}

	// job termination
	delete visManager;
	delete runManager;
	return 0;
}