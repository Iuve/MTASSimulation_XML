#include "MTASSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4UserSteppingAction.hh"
#include "MTASGlobals.hh"
#include "G4RunManager.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include <stdio.h>
#include <math.h>
#include <string>

MTASSteppingAction::MTASSteppingAction( )
{
}

MTASSteppingAction::~MTASSteppingAction()
{
}

void MTASSteppingAction::UserSteppingAction(const G4Step* theStep)
{

  G4StepPoint* preStepPoint = theStep->GetPreStepPoint();
  G4double light;
 
	G4TouchableHandle preStepTouch = preStepPoint->GetTouchableHandle();
	G4VPhysicalVolume* preStepPV = preStepTouch->GetVolume();
	G4LogicalVolume* preStepLV = preStepPV->GetLogicalVolume();
	std::string materialName = preStepLV ->GetMaterial()->GetName(); 
	
	G4Track* track = theStep->GetTrack();
	G4double globalTime = track -> GetGlobalTime();
	
	//Get first in event interaction time (in nanoseconds) and process
	/*const G4String& processName = theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	G4int trackID = track->GetTrackID();
	
	if( processName != "Transportation" && trackID == 1)
	{
		//std::cout << globalTime << " " << globalTime / ns << " " << globalTime / s << " " << processName << std::endl;
		std::cout << globalTime << " " << processName << std::endl;
		track->SetTrackStatus(fStopAndKill);
	}
	*/
	
	
	if(globalTime / s > g_eventInSeconds)
		{
		track->SetTrackStatus(fStopAndKill);
		} 
		
		
	 
	if(materialName == "SodiumIodide")
	{
		light = FindLight(theStep);
		
//  
/*
		G4String name   = particle->GetParticleName();
		G4String type   = particle->GetParticleType(); 
		
		G4int copyNumber = preStepTouch -> GetCopyNumber(4);

	G4int eID = 0;
	const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
	if(evt) eID = evt->GetEventID();

	
	G4int stepNumber = track -> GetCurrentStepNumber();
	const G4String& processName = theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	G4double kinEnergy = track->GetKineticEnergy();
	
	const G4DynamicParticle* dynParticle = track->GetDynamicParticle();
	G4ParticleDefinition* particle = dynParticle->GetDefinition();
	G4double particleKinEnergy = dynParticle->GetKineticEnergy();
	G4String particleName = particle->GetParticleName();
	// kinEnergy from track == kinEnergy from dynamic particle
	G4double depEnergy = theStep->GetTotalEnergyDeposit(); 
	
	G4double deltaEnergyStep = theStep->GetDeltaEnergy();
	G4int trackID = track->GetTrackID();
*/
//	if (globalTime / s > eventInSeconds)
//	{
//		G4cout << "eventID: " << eID << " trackID: " << trackID << " kinEnergy: " << kinEnergy / keV << " depEnergy: " << depEnergy / keV <<
//	" deltaE step: " << deltaEnergyStep / keV << " " << processName << " " << particleName << " globalTime(ns): " << globalTime / ns << G4endl;
//	}


		//number of module
		if(preStepPV->GetName() == "NaI_Centre_Physical")
			lightProdInNaIMod[0] += light;
		else
		{
			G4int copyNumber = preStepTouch -> GetCopyNumber(4);//4 - grand grand grandmother
			if(copyNumber < 18)
				lightProdInNaIMod[copyNumber+1] += light;
			else
				std::cout<<"error: copy number = "<<copyNumber<<std::endl;
		}
	}
}



G4double MTASSteppingAction::FindLight(const G4Step* theStep)
{
	G4StepPoint* preStepPoint = theStep->GetPreStepPoint();
  G4double depEnergy = theStep->GetTotalEnergyDeposit(); 
	G4double preStepTotEnergy = preStepPoint -> GetKineticEnergy();
	return (LightForumla(preStepTotEnergy*1000)-LightForumla(preStepTotEnergy*1000-depEnergy*1000));
}

G4double MTASSteppingAction::LightForumla(G4double E)
{
	G4double a1 = 1.6;
	G4double a2 = 0.058;
	G4double a3 = 0.58;
	G4double a4 = 0.0049;
	G4double a5 = 0.25;
	G4double a6 = 0.479;
	G4double a7 = 0.00494;

	G4double light = E* (a1*(1.- exp(-a2*E)) + a3*E + a4*E*E)/(a5 + a6*E + a7*E*E);
	return light;
	//return E;
}

std::vector <G4double> MTASSteppingAction::lightProdInNaIMod(19,0);

