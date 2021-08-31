#ifndef PhysicsList_HH
#define PhysicsList_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalParameters.hh"

class PhysicsList : public G4VModularPhysicsList
{
    public:
        PhysicsList();
        ~PhysicsList();


};

#endif