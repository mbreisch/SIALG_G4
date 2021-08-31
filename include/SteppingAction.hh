#ifndef SteppingAction_HH
#define SteppingAction_HH

#include "G4UserSteppingAction.hh"
#include "EventAction.hh"

class SteppingAction : public G4UserSteppingAction
{
	public:
		SteppingAction();
		~SteppingAction();

};


#endif