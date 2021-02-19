
#include "DeclareHeaders.hh"

#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include <vector>
#include <iostream>

double g_eventInSeconds;
void SetEventLength(double time) { g_eventInSeconds = time; }

Decay::Decay()
{
	loadDecayData_ = new LoadDecayData();
	allNuclides_ = loadDecayData_->GetAllNuclides();
	startLevel_ = loadDecayData_->GetStartLevel();
	stopLevel_ = loadDecayData_->GetStopLevel();
	cycleDurationInSeconds_ = loadDecayData_->GetCycleDurationInSeconds();
	eventDurationInSeconds_ = loadDecayData_->GetEventDurationInSeconds();
	isomerLevel_ = 0L;
	eventTimeInSeconds_ = 0.;
	g_eventInSeconds = eventDurationInSeconds_;
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
		currentLevel = isomerLevel_;
		isomerLevel_ = 0L;
		
		if(!IfEmissionFromLevel(currentLevel, cycleDurationInSeconds_ - eventTimeInSeconds_))
		{
			eventTimeInSeconds_ = 0.; // end of event, no further transitions, zero time
			//eventList_ is always empty at this moment so new decay can be started here
			
			currentLevel = startLevel_; // new decay on behalf of "old" Geant4 event
			FindEventTime(currentLevel);
		}
	}

	Level* nextLevel;
	std::vector<Transition*>* transitionsForCheck;
	transitionsForCheck = ( currentLevel->GetTransitions() );

	while ( transitionsForCheck->size() && currentLevel != stopLevel_) // empty Transitions or STOP level
	{
		nextLevel = FindTransition(currentLevel); // adding events to list in this line
		currentLevel = nextLevel;
		
		if(!IfEmissionFromLevel(currentLevel, eventDurationInSeconds_))
		{
			isomerLevel_ = currentLevel;
			eventTimeInSeconds_ += eventDurationInSeconds_;
			
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
	if(T12 <= 0.)
		eventTimeInSeconds_ = 0.;
	else
	{
		double lambda = log(2.)/T12;
		double emissionProbability = 1. - exp(-lambda * cycleDurationInSeconds_);
		double pseudoRandomProbability = G4UniformRand() * emissionProbability;
		eventTimeInSeconds_ = -log(1. - pseudoRandomProbability) / lambda;
	}
}

bool Decay::IfEmissionFromLevel(Level* level, double timeGate) // Poisson distribution
{
	double T12 = level->GetHalfLifeTime(); // in seconds
	if(T12 <= 0.)
		return true;
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
	std::vector<Transition*>* transitionsFromLevel = fromLevel->GetTransitions();
	
	for ( int i = 0; i < transitionsFromLevel->size(); ++i)
	{
		if (randomNumber < transitionsFromLevel->at(i)->GetRunningIntensity())
		{
			AddEvent( transitionsFromLevel->at(i) );
			return transitionsFromLevel->at(i)->GetPointerToFinalLevel();
		}
	}
	std::cout << "Unsuccessful finding of next transition" << std::endl;
	return fromLevel;
} 

void Decay::AddEvent (Transition* drawnTransition)
{
	std::string particleType = drawnTransition->GetParticleType();
	double energy = drawnTransition->GetTransitionQValue();
	
	if(particleType == "B-" || particleType == "B+" || particleType == "EC") 
	{
		std::vector<Event> betaEvents = drawnTransition->FindBetaEvent();
		eventList_.insert(eventList_.end(), betaEvents.begin(), betaEvents.end()); 
	}
/*	if(particleType == "B+") 
	{
		std::vector<Event> betaEvents = drawnTransition->FindBetaEvent();
		eventList_.insert(eventList_.end(), betaEvents.begin(), betaEvents.end()); 
	}
*/
	if(particleType == "G") 
	{
		if ( drawnTransition->GetElectronConversionCoefficient() == 0L) //do sprawdzenia czy dziala!!
		{
			G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
			eventList_.push_back(Event(energy, particleTable->FindParticle("gamma")));
		}
		else
		{
			std::vector<Event> gammaICEvents = drawnTransition->FindGammaEvents();
			eventList_.insert(eventList_.end(), gammaICEvents.begin(), gammaICEvents.end());
		}
	}
	if(particleType == "N") 
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		eventList_.push_back(Event(energy, particleTable->FindParticle("neutron")));
	}
	if(particleType == "A") 
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		eventList_.push_back(Event(energy, particleTable->FindParticle("alpha")));
	}
	if(particleType == "P") 
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		eventList_.push_back(Event(energy, particleTable->FindParticle("proton")));
	}
}


