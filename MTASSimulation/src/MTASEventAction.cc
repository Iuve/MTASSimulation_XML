
#include "MTASDetectorConstruction.hh"
#include "G4HCofThisEvent.hh"

#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4VHit.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

 
#include "MTASEventAction.hh"
#include "MTASPrimaryGeneratorAction.hh"
#include "MTASSteppingAction.hh"
#include <stdio.h>
#include <string>
MTASEventAction::MTASEventAction(std::string outputFile): m_AnalysisManager(0),colIDSumCenter(0)
{
	StartUp(outputFile);
}
 
MTASEventAction::~MTASEventAction()
{
	CleanUp();
}
 
void MTASEventAction::StartUp(std::string outputFile)
{
	CleanUp();
	m_AnalysisManager = new MTASAnalysisManager(outputFile);
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
  
	G4String Name1="NaIOuter/eDep";
	G4String Name2="NaICenter/eDep";
	G4String Name3="Sili/eDep";
  
	colIDSumCenter = SDman->GetCollectionID(Name2);
	colIDSumOuter = SDman->GetCollectionID(Name1);
	colIDSumSili = SDman->GetCollectionID(Name3);
}

void MTASEventAction::CleanUp()
{
	if( m_AnalysisManager != 0 )
	{
		delete m_AnalysisManager;
	}
}


void MTASEventAction::BeginOfEventAction(const G4Event* )
{
	//m_AnalysisManager->AddEnergyDepositedInCell( m_primaryActionGenerator->GetEnergy());
}

 
void MTASEventAction::EndOfEventAction(const G4Event* evt)
{
	G4int event_id = evt->GetEventID();
	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	if(!HCE) return;

//sensitives detectors
	G4THitsMap<G4double>* evtMapOuter = (G4THitsMap<G4double>*)(HCE->GetHC(colIDSumOuter));
	G4THitsMap<G4double>* evtMapCenter = (G4THitsMap<G4double>*)(HCE->GetHC(colIDSumCenter));
	G4THitsMap<G4double>* evtMapSili = (G4THitsMap<G4double>*)(HCE->GetHC(colIDSumSili));
	//G4THitsMap<G4double>* evtMapShield = (G4THitsMap<G4double>*)(HCE->GetHC(colIDSumShield));

	//mapSumOuter = (*evtMapOuter); 
	//mapSumCenter = (*evtMapCenter);
	//mapSumSili = (*evtMapSili);
	//mapSumShield = (*evtMapShield);
	 
	G4double TotalEnergy=0.0;
	G4double CentralEn=0.0;
	G4double MiddleEn=0.0;
	G4double ExternalEn=0.0;
	//G4double ShieldEn=0.0;
	G4double SiliEn[14];
	G4double TotalSiliEn =0.;
	G4double ModuleEn[19];

	for(int i=0;i<18;i++)
	{ 
		ModuleEn[i+1]=0.0;
		if((*evtMapOuter)[i]!=0)
		{
			TotalEnergy+=*((*evtMapOuter)[i]);
			ModuleEn[i+1]=*((*evtMapOuter)[i]);
			if(i<6) //middle ring
				MiddleEn+=*((*evtMapOuter)[i]);
			else //external ring
				ExternalEn+=*((*evtMapOuter)[i]);
		}
	}
	
	ModuleEn[0]=0.0;
	if((*evtMapCenter)[0] != 0)
	{
		ModuleEn[0] = *((*evtMapCenter)[0]);
		TotalEnergy += *((*evtMapCenter)[0]);
		CentralEn += *((*evtMapCenter)[0]);
	}
  
  for(int i=0;i<14;i++)
	{ 	
		SiliEn[i] =0.;
		if((*evtMapSili)[i] !=0)
		{
			SiliEn[i] = *((*evtMapSili)[i]);
			TotalSiliEn += *((*evtMapSili)[i]);
		}
  } 

	m_AnalysisManager->AddEnergyDepositedInCell(event_id,TotalEnergy,ModuleEn);
	m_AnalysisManager->AddEnergyDepositedInCell2(event_id,TotalEnergy,ModuleEn);
	m_AnalysisManager->AddSiliconEnergy1(event_id, TotalSiliEn,SiliEn);
	m_AnalysisManager->AddSiliconEnergy2(event_id, TotalSiliEn,SiliEn);


	G4double sumLight = 0;
	for(int i=0; i<19; i++)
		sumLight += MTASSteppingAction::lightProdInNaIMod.at(i);	
  	
	m_AnalysisManager->AddLight1(event_id , sumLight, MTASSteppingAction::lightProdInNaIMod);
  	m_AnalysisManager->AddLight2(event_id , sumLight, MTASSteppingAction::lightProdInNaIMod);	
	
  	for(int i=0; i<19; i++)
  		MTASSteppingAction::lightProdInNaIMod[i] = 0;	
  	
	
// THIS IS FOR GEANT OUTPUT AND VISUALIZATION OUT, NO HISTOGRAMS INFO DOWN HERE
// In order for this to work you must set the "/tracking/storeTrajectory 1" in the macro. 
// For us this is turned on in the vis.mac
// get number of stored trajectories
	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	if( trajectoryContainer ) n_trajectories = trajectoryContainer->entries();
  
	if( false ) // BE CAREFUL, this can output a SHITLOAD of data
	{
		for( G4int i=0; i < n_trajectories; i++ ) 
		{
			G4Trajectory* trj = (G4Trajectory*)( (*( evt->GetTrajectoryContainer() ) )[i] );
			const G4String particleName = trj->GetParticleName();
			const int pdgenc = trj->GetPDGEncoding();
			if( pdgenc == 22 ) // 22 = gamma, 11 = electron
			{
				G4ThreeVector initMom = trj->GetInitialMomentum();
				G4double initMomSq = initMom[0] * initMom[0] + initMom[1] * initMom[1] + initMom[2] * initMom[2];
				if( 40.0 * 40.0 * keV * keV < initMomSq )
				{
					G4PrimaryVertex* primaryVertex = evt->GetPrimaryVertex();
					G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
					G4ThreeVector particleLaunchMom = primaryParticle->GetMomentum();
					G4double PT = sqrt( particleLaunchMom[0] * particleLaunchMom[0] +
					particleLaunchMom[1] * particleLaunchMom[1] + particleLaunchMom[2] * particleLaunchMom[2] );
					
					const G4double initmom = sqrt( initMomSq );
					G4cout << PT / keV << ", " << initmom / keV << "," << G4endl;
				}
			}
		}
	}


// periodic printing every 100 events
	if( event_id % 100 == 0 )
	{
		std::cout << "Finished Running Event # " << event_id << "\r" << std::flush; // G4endl;
	}
    
// extract the trajectories and draw them
	if( G4VVisManager::GetConcreteInstance() )
	{
	   for( G4int i=0; i < n_trajectories; i++ ) 
		{ 
			G4Trajectory* trj = (G4Trajectory*)( (*( evt->GetTrajectoryContainer() ))[i] );
			trj->DrawTrajectory();
		}
	}
}


