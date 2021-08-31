#ifndef ActionInitialization_HH
#define ActionInitialization_HH

#include "G4VUserActionInitialization.hh"

#include <Generator.hh>
#include <RunAction.hh>
#include <EventAction.hh>
#include <SteppingAction.hh>

class ActionInitialization : public G4VUserActionInitialization
{
	public:
		ActionInitialization();
		~ActionInitialization();

		virtual void Build() const;

};


#endif