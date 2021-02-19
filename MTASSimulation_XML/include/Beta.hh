
#ifndef NEW_BETA_H
#define	NEW_BETA_H

#include "FermiDistribution.hh"
#include "G4DataVector.hh"
#include "G4AtomicTransitionManager.hh"
#include <string>


class Beta : public Transition
{
public:
	Beta(std::string particleType, double transitionQValue, double intensity,
	 double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber);
	 
	~Beta();
	
	void SetECCoef(std::string type, double value);
	std::vector<Event> FindBetaEvent();
	
	
	
private:

	int FindPrimaryVacancies();
	void AddXRaysEvent(std::vector<Event> &decay, int primaryVacancies);
	void AddAugerEvent(std::vector<Event> &decay, int primaryVacancies);
	int FindRandomIndex( const G4DataVector transProb);

	int atomicNumber_;
	FermiDistribution betaEnergyDistribution_;
	double ECCoeff_[4];
	
	G4AtomicTransitionManager* atomicTransitionManager_;
};

#endif
