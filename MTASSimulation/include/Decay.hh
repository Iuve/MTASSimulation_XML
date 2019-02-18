
#ifndef NEW_DECAY_H
#define	NEW_DECAY_H
#include "G4ParticleDefinition.hh"
#include "LoadDecayData.hh"
#include "Nuclide.hh"
#include "Level.hh"
#include "Transition.hh"
#include "FermiDistribution.hh"
#include <vector>
#include <string>

class Nuclide;
class Level;
class Transition;
class LoadDecayData;

struct Event
{
	double  energy;
	G4ParticleDefinition* type;
	Event(double energyVal, G4ParticleDefinition* typeVal):
	energy(energyVal), type(typeVal){}
	
};

class Decay
{
public:
	Decay(); // loading and preparing data
	~Decay();
	std::vector<Event> GenerateEventList();
	void AddEvent(std::string, double, FermiDistribution*);
	
	Level* FindTransition(Level*);
	
	//std::vector<Event> FindGammaEvents(Transition transition);
	
	
private:	
	
	//bool IsGammaDecay(double electronConversionCoefficient);

	Level* startLevel_;
	Level* stopLevel_;
	std::vector<Nuclide>* allNuclides_;
	std::vector<Event> eventList_;
	LoadDecayData* loadDecayData_;
	static Level* isomerLevel_;
	double eventTimeInSeconds_;
	
	void FindEventTime(Level*);
	bool IfEmissionFromLevel(Level*, double);
};

#endif	/* DECAY_H */
