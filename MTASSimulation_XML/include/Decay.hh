
#ifndef NEW_DECAY_H
#define	NEW_DECAY_H

#include "G4ParticleDefinition.hh"

#include <vector>
#include <string>

extern double g_eventInSeconds;
extern double g_cycleDurationInSeconds;

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
	void AddEvent(Transition*);
	
	Level* FindTransition(Level*);
	
	
private:	

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
