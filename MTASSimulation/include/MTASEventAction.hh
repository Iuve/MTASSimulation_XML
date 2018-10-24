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
//
 
#ifndef MTASEventAction_h
#define MTASEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"
#include "G4Event.hh"

#include "MTASAnalysisManager.hh"


class MTASEventAction : public G4UserEventAction
{
  public:
	MTASEventAction(std::string outputFile);
   ~MTASEventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
	
  private:
    void StartUp(std::string outputFile);
    void CleanUp();
	
		MTASAnalysisManager* m_AnalysisManager;
		G4THitsMap<G4double> mapSumOuter;
    G4THitsMap<G4double> mapSumCenter;
    G4THitsMap<G4double> mapSumSili;
    G4THitsMap<G4double> mapSumShield;
		
    G4int colIDSumOuter;
    G4int colIDSumCenter;
    G4int colIDSumSili;
    G4int colIDSumShield;
	
};

#endif

    
