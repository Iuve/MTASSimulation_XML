
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
	void CalculateBetaStuff();
	void CheckBetaTransitionType();

	int atomicNumber_;
	FermiDistribution betaEnergyDistribution_;
	double ECCoeff_[4];
	
//	betaTransitionType = 0: allowed Fermi and GT
//  GT : Gamow-Teller Decay
//  betaTransitionType = 1: 0-: pe^2  + Eν^2 + 2β^2 * Eν * Ee
//  betaTransitionType = 2: 1-: pe^2  + Eν^2 - 4/3 * β^2 * Eν * Ee
//  betaTransitionType = 3: 2-: pe^2  + Eν^2 
//	F : Fermi Decay
//  betaTransitionType = 4: 1-: pe^2  + Eν^2 + 2/3 β^2 * Eν * Ee
	int betaTransitionType_;
	bool calculationsDone_;
	
	G4AtomicTransitionManager* atomicTransitionManager_;
};

#endif
