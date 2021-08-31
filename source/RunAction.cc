#include "RunAction.hh"

RunAction::RunAction()
{
	std::string tmp_path = "../SimulationResults/Run_";
	itr = 0;
	itr_name = tmp_path + std::to_string(itr);

	while(IsPathExist(itr_name))
	{
		itr++;
		itr_name = tmp_path + std::to_string(itr);  
	}

	std::string cmd_folder = "mkdir -p ";
	cmd_folder += itr_name;
	int systemRet = system(cmd_folder.c_str());
	if(systemRet == -1)
	{
		G4cout << "No folder was created" << G4endl;
	}
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager *AnaMan = G4AnalysisManager::Instance();

  	std::string fileprefix = "/Run-" + std::to_string(itr) + "_SubRun-";
  	std::string fileend = ".root";

  	int subrun = 0;
  	filename = itr_name + fileprefix + std::to_string(subrun) + fileend;

  	while(IsPathExist(filename))
  	{
  		subrun++;
  		filename = itr_name + fileprefix + std::to_string(subrun) + fileend;
  	}

	AnaMan->OpenFile(filename);

	AnaMan->CreateNtuple("Hits", "Hits");
	AnaMan->CreateNtupleIColumn("fEvent");
	AnaMan->CreateNtupleDColumn("fX");
	AnaMan->CreateNtupleDColumn("fY");
	AnaMan->CreateNtupleDColumn("fZ");
	AnaMan->CreateNtupleDColumn("ftGlob");
	AnaMan->CreateNtupleDColumn("ftLoc");
	AnaMan->CreateNtupleDColumn("ftProp");
	AnaMan->CreateNtupleDColumn("fKinEnergy");
	AnaMan->CreateNtupleDColumn("fTotEnergy");
	AnaMan->CreateNtupleDColumn("ptGlob");
	AnaMan->CreateNtupleDColumn("ptLoc");
	AnaMan->CreateNtupleDColumn("ptProp");
	AnaMan->CreateNtupleDColumn("pKinEnergy");
	AnaMan->CreateNtupleDColumn("pTotEnergy");
	AnaMan->FinishNtuple(0);
}

void RunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *AnaMan = G4AnalysisManager::Instance();

	AnaMan->Write();

	AnaMan->CloseFile((G4String)filename);
}

bool RunAction::IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}