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

#include <math.h>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UItcsh.hh"
#include "G4UIterminal.hh"
#include "G4VisManager.hh"

#include "MTASDetectorConstruction.hh"
#include "MTASPhysicsList.hh"
#include "MTASPrimaryGeneratorAction.hh"
#include "MTASVisualizationManager.hh"
#include "MTASEventAction.hh"
#include "MTASSteppingAction.hh"
#include "MTASAnalysisManager.hh" 
#include "G4ios.hh"

#include <stdio.h>
#include <iostream>
#include <fstream>

extern void SetDecaySimulationStatus(bool);
extern void SetXmlInputFileName(std::string);

using namespace std;

int main(int argc,char** argv)
{

  // Run manager
  G4RunManager * runManager = new G4RunManager;
	runManager->SetVerboseLevel(2);
// UserInitialization classes (mandatory)
  MTASPhysicsList* physicsList = new MTASPhysicsList;
  runManager->SetUserInitialization( physicsList );
  
  MTASDetectorConstruction* detector = new MTASDetectorConstruction;
  runManager->SetUserInitialization( detector );

  
//  Initialize G4 kernel Can this be set before the user run actions are set?
//  Needs to because sesitive detectors are created here and are looked for in NuSNSEventAction. 
  runManager->Initialize();
  
// Visualization
#ifdef G4VIS_USE
  MTASVisualizationManager* visManager = new MTASVisualizationManager;
  visManager->Initialize();
//  G4VisManager* visManager = new G4VisExecutive;
#endif
	
// Global values definition
	if(argc == 3) 
	{
		SetDecaySimulationStatus(true);
		SetXmlInputFileName(argv[2]);
	}
	
// UserAction classes
	MTASPrimaryGeneratorAction* primaryGeneratorAction = new MTASPrimaryGeneratorAction(detector);
	MTASEventAction* eventAction = new MTASEventAction("output.root");
  MTASSteppingAction* userSteppingAction = new MTASSteppingAction();


  runManager->SetUserAction( primaryGeneratorAction );
  runManager->SetUserAction( eventAction );
  runManager->SetUserAction( userSteppingAction );


      
  //get the pointer to the User Interface manager 
  G4UImanager * UI = G4UImanager::GetUIpointer();  

  if(argc==1) // Define (G)UI terminal for interactive mode  
  {
    G4UIsession * session = 0;
		#ifdef G4UI_USE_TCSH
			session = new G4UIterminal( new G4UItcsh );
		#else
			session = new G4UIterminal(); // G4UIterminal is a (dumb) terminal.
		#endif

    UI->ApplyCommand("/control/execute vis.mac");
		session->SessionStart();
    delete session;
  }
  else
  // Batch mode
  { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }
  
// Clean up
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
//  delete verbosity;
  return 0;
}


//*************************************************************************
