//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
#ifndef MTASSteppingAction_h
#define MTASSteppingAction_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include <vector>

class MTASSteppingAction : public G4UserSteppingAction
{
public:
	MTASSteppingAction();
  ~MTASSteppingAction();
	virtual void UserSteppingAction( const G4Step* );
  
	static std::vector <double> lightProdInNaIMod;
	
	//static G4double *lightProdInNaIMod;

private:
	G4double FindLight(const G4Step* theStep);
	G4double LightForumla(G4double E);
	G4int nSecondaryByLastStep;

};

#endif // MTASSteppingAction_h
