
#include "Decay.hh"
#include "Beta.hh"
#include "LoadDecayData.hh"
#include "Level.hh"
#include "Transition.hh"
#include "FermiDistribution.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "Globals.hh"

#include <vector>
//#include <cstdlib>
//#include <ctime>
#include <iostream>


Decay::Decay()
{

	loadDecayData_ = new LoadDecayData();
	allNuclides_ = loadDecayData_->GetAllNuclides();
	startLevel_ = loadDecayData_->GetStartLevel();
	stopLevel_ = loadDecayData_->GetStopLevel();
	isomerLevel_ = 0L;
}


Decay::~Decay()
{
	if(loadDecayData_)
		delete loadDecayData_;
}

 

std::vector<Event> Decay::GenerateEventList() 
{
	if( !eventList_.empty() )
		eventList_.clear();
	Level* currentLevel;

	if(isomerLevel_ == 0L) //there is not "lefted" isomer from previous decay
		currentLevel = startLevel_;
	else
	{
		if(!isomerLevel_->IfEmissionFromLevel(g_cycleDurationInSeconds))
			return eventList_;
		currentLevel = isomerLevel_;
		isomerLevel_ = 0L;
	}

	Level* nextLevel;
	std::vector<Transition>* transitionsForCheck;
	transitionsForCheck = ( currentLevel->GetTransitions() );

	while ( transitionsForCheck->size() && currentLevel != stopLevel_) // pusty wektor z Transitions, STOP lub T1/2 jako globalna zmienna
	{
		nextLevel = FindTransition(currentLevel);
		currentLevel = nextLevel;
		
		if(!currentLevel->IfEmissionFromLevel(g_eventInSeconds))
		{
			isomerLevel_ = currentLevel;
			if (isomerLevel_ == stopLevel_)
			{
				isomerLevel_ = 0L;
			}
			break;	
		}
		transitionsForCheck = ( currentLevel->GetTransitions() );
	};
	return eventList_;	
}

Level* Decay::FindTransition(Level* fromLevel) // fromLevel ???
{
	double randomNumber = G4UniformRand() * fromLevel->GetTotalIntensity();
	
	for ( auto it = fromLevel->GetTransitions()->begin(); it != fromLevel->GetTransitions()->end(); ++it )
	{
		if (randomNumber < it->GetRunningIntensity())
		{
			if ( it->GetParticleType() == "G" && it->GetElectronConversionCoefficient() > 0.)
			{
				std::vector<Event> gammaICEvents = it->FindGammaEvents();
				eventList_.insert(eventList_.end(), gammaICEvents.begin(), gammaICEvents.end());
			}
			else
				AddEvent(it->GetParticleType(), it->GetTransitionQValue(), it->GetBetaEnergyDistribution());
			return it->GetPointerToFinalLevel();
		}
	}
} 

void Decay::AddEvent (std::string particleType, double energy, FermiDistribution* betaEnergyDistribution)
{
	if(particleType == "B-") 
	{
		Beta temp(energy, -1, betaEnergyDistribution); 	
		std::vector<Event> betaEvents = temp.GetBetaEvents();

		for(unsigned int i=0; i<betaEvents.size(); ++i)
			{
				eventList_.push_back(betaEvents.at(i));
			}
	}
	if(particleType == "B+") 
	{
		Beta temp(energy, 1, betaEnergyDistribution);
		std::vector<Event> betaEvents = temp.GetBetaEvents();
		eventList_.insert(eventList_.end(), betaEvents.begin(), betaEvents.end());
//		eventList_.push_back(temp->GetBetaEvents());
	}
	if(particleType == "G") 
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		eventList_.push_back(Event(energy, particleTable->FindParticle("gamma")));
		//eventList_.push_back(Event(energy, "gamma"));
	}
	if(particleType == "N") 
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		eventList_.push_back(Event(energy, particleTable->FindParticle("neutron")));
		//eventList_.push_back(Event(energy, "neutron"));
	}
}

Level* Decay::isomerLevel_ = 0L;
