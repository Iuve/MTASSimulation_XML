

#ifndef NEW_LEVEL_H
#define	NEW_LEVEL_H
#include "Nuclide.hh"
#include "Transition.hh"
#include <vector>
#include <string>

class Transition;
class Level
{
public:
	Level(double levelEnergy, double spin, std::string parity, double halfLifeTimeInSeconds, std::vector<Transition> transitions); 
	~Level();
	double GetTotalIntensity(){return totalIntensity_;}
	
	double GetLevelEnergy(){return levelEnergy_;}
	std::vector<Transition>* GetTransitions(){return &transitions_;}
//	double GetHalfLifeTime(){return halfLifeTime_;}
	bool IfEmissionFromLevel(double);
	

private:
	double levelEnergy_;
	double spin_;
	std::string parity_; // string or int -1, 1 ??
	std::vector<Transition> transitions_;
	double halfLifeTimeInSeconds_;
	double totalIntensity_;
	void CalculateTotalProbability();
};

#endif	/* DECAY_H */
