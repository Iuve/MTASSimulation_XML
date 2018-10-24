

#ifndef MTASPhysicsList_h
#define MTASPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class MTASPhysicsList: public G4VModularPhysicsList
{
  public:
    MTASPhysicsList();
    ~MTASPhysicsList();

  public:

    virtual void SetCuts();
};

#endif // MTASPhysicsList_h
