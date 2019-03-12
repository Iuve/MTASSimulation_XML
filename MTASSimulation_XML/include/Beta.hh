
#include "FermiDistribution.hh"
#include "Decay.hh"
#include <string>


class Beta
{
public:
	Beta(double, int, FermiDistribution*);
	~Beta();
	
	double GetMaxBetaEnergy(){return maxBetaEnergy_;}
	void FindBetaEvent();
	std::vector<Event> GetBetaEvents(){return betaEvents_;}
	
	
	
private:
	double maxBetaEnergy_;
	int eCharge_;
	std::vector<Event> betaEvents_;
    
	FermiDistribution* betaEnergyDistribution_;





};
