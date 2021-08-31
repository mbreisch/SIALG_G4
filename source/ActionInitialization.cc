#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
	Generator *gen = new Generator();
	SetUserAction(gen);

	RunAction* runAction = new RunAction();
	SetUserAction(runAction);
/*
	EventAction* eventAction = new EventAction(runAction);
	SetUserAction(eventAction);

	SteppingAction *stepAction = new SteppingAction(eventAction);

	SetUserAction(stepAction);
*/
}