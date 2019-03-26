
#include "Decay.hh"
#include "Beta.hh"
#include "LoadDecayData.hh"
#include "Level.hh"
#include "Transition.hh"
#include "FermiDistribution.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "MTASGlobals.hh"

#include <vector>
#include <iostream>


Decay::Decay()
{

	loadDecayData_ = new LoadDecayData();
	allNuclides_ = loadDecayData_->GetAllNuclides();
	startLevel_ = loadDecayData_->GetStartLevel();
	stopLevel_ = loadDecayData_->GetStopLevel();
	isomerLevel_ = 0L;
	eventTimeInSeconds_ = 0.;
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
	{
		currentLevel = startLevel_;
		FindEventTime(currentLevel);
	}
	else
	{
		if(!IfEmissionFromLevel(isomerLevel_, g_cycleDurationInSeconds - eventTimeInSeconds_))
		{
			eventTimeInSeconds_ = 0.; // end of event, no further transitions, zero time
			return eventList_;
		}
		currentLevel = isomerLevel_;
		isomerLevel_ = 0L;
	}

	Level* nextLevel;
	std::vector<Transition>* transitionsForCheck;
	transitionsForCheck = ( currentLevel->GetTransitions() );

	while ( transitionsForCheck->size() && currentLevel != stopLevel_) // empty Transitions or STOP level
	{
		nextLevel = FindTransition(currentLevel);
		currentLevel = nextLevel;
		
		if(!IfEmissionFromLevel(currentLevel, g_eventInSeconds))
		{
			isomerLevel_ = currentLevel;
			eventTimeInSeconds_ += g_eventInSeconds;
			
			if (isomerLevel_ == stopLevel_)
			{
				eventTimeInSeconds_ = 0.;
				isomerLevel_ = 0L;
			}
			break;	
		}
		transitionsForCheck = ( currentLevel->GetTransitions() );
	};
	
	if(isomerLevel_ == 0L)
		eventTimeInSeconds_ = 0.;
		
	return eventList_;	
}

void Decay::FindEventTime(Level* level)
{
	double T12 = level->GetHalfLifeTime(); // in seconds
	double lambda = log(2.)/T12;
	double emissionProbability = 1. - exp(-lambda * g_cycleDurationInSeconds);
	double pseudoRandomProbability = G4UniformRand() * emissionProbability;
	eventTimeInSeconds_ = -log(1. - pseudoRandomProbability) / lambda;
}

bool Decay::IfEmissionFromLevel(Level* level, double timeGate) // Poisson distribution
{
	double T12 = level->GetHalfLifeTime(); // in seconds
	double lambda = log(2.)/T12;
	double emissionProbability = 1. - exp(-lambda * timeGate);
	double randomNumber = G4UniformRand();
	if (randomNumber <= emissionProbability)
	{
		eventTimeInSeconds_ += -log(1. - randomNumber * emissionProbability) / lambda;
		return true;
	}
	else
		return false;
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
