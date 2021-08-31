#ifndef RunAction_HH
#define RunAction_HH

#include "G4UserRunAction.hh"
#include "g4root.hh"
#include <string>

class RunAction :public G4UserRunAction
{
	public:
		RunAction();
		~RunAction();

		virtual void BeginOfRunAction(const G4Run*);
		virtual void EndOfRunAction(const G4Run*);
	private:
		std::string itr_name;
		std::string filename;
		int itr;
		bool IsPathExist(const std::string &s);
};


#endif