#include "SipmArray.hh"

SipmArray::SipmArray(G4String name) : G4VSensitiveDetector(name)
{}

SipmArray::~SipmArray()
{}

G4bool SipmArray::ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist)
{
	G4Track *track = aStep->GetTrack();

	track->SetTrackStatus(fStopAndKill);

	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4int IdNumber = touchable->GetCopyNumber();

	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();

	G4double tGlob = preStepPoint->GetGlobalTime();
	G4double tLoc = preStepPoint->GetLocalTime();
	G4double tProp = preStepPoint->GetProperTime();
	G4double kinEnergy = preStepPoint->GetKineticEnergy();
	G4double totEnergy = preStepPoint->GetTotalEnergy();

	G4double p_tGlob = postStepPoint->GetGlobalTime();
	G4double p_tLoc = postStepPoint->GetLocalTime();
	G4double p_tProp = postStepPoint->GetProperTime();
	G4double p_kinEnergy = postStepPoint->GetKineticEnergy();
	G4double p_totEnergy = postStepPoint->GetTotalEnergy();

	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	G4AnalysisManager *AnaMan = G4AnalysisManager::Instance();
	AnaMan->FillNtupleIColumn(0, evt);
	AnaMan->FillNtupleDColumn(1,posDetector[0]);
	AnaMan->FillNtupleDColumn(2,posDetector[1]);
	AnaMan->FillNtupleDColumn(3,posDetector[2]);
	AnaMan->FillNtupleDColumn(4,tGlob);
	AnaMan->FillNtupleDColumn(5,tLoc);
	AnaMan->FillNtupleDColumn(6,tProp);
	AnaMan->FillNtupleDColumn(7,kinEnergy);
	AnaMan->FillNtupleDColumn(8,totEnergy);
	AnaMan->FillNtupleDColumn(9,p_tGlob);
	AnaMan->FillNtupleDColumn(10,p_tLoc);
	AnaMan->FillNtupleDColumn(11,p_tProp);
	AnaMan->FillNtupleDColumn(12,p_kinEnergy);
	AnaMan->FillNtupleDColumn(13,p_totEnergy);
	AnaMan->AddNtupleRow(0);

	return(0);
}