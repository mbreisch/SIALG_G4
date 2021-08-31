#ifndef SipmArray_HH
#define SipmArray_HH


#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

class SipmArray : public G4VSensitiveDetector 
{
    public:
        SipmArray(G4String);
        ~SipmArray();

        
    private:
    	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

};

#endif