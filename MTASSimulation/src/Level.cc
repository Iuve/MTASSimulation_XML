

#include "Level.hh"
#include "Decay.hh"
#include <vector>
#include <string>
#include "Randomize.hh"

Level::Level(double levelEnergy, double spin, std::string parity, double halfLifeTimeInSeconds, std::vector<Transition> transitions):
levelEnergy_(levelEnergy), spin_(spin), parity_(parity), halfLifeTimeInSeconds_(halfLifeTimeInSeconds), transitions_(transitions)
{
	CalculateTotalProbability();
	std::cout << "Total Probability: " << totalIntensity_ << std::endl;
	std::cout << "Level energy: " << levelEnergy_ << std::endl;
}


Level::~Level()
{

}

void Level::CalculateTotalProbability()
{
	totalIntensity_ = 0.;
	for ( auto it = transitions_.begin(); it != transitions_.end(); ++it  ) // c++ 11
	{
		totalIntensity_ += it->GetIntensity();
		it->SetRunningIntensity(totalIntensity_);
	}

}

/*bool Level::IsIsomerLevel()
{
	/**Time limit for isomer state, if time of life is higher than the limit
	level is treated as an isomer state**/
/*	const double timeLimitInSec = 2.;//What is good value?
	if (halfLifeTimeInSeconds_ > timeLimitInSec)
		return true;
	else
		return false;
} */

bool Level::IfEmissionFromLevel(double timeGate) // Poisson distribution
{
	double lambda = log(2.)/halfLifeTimeInSeconds_;
	double emissionProbability = 1. - exp(-lambda * timeGate);
	if (G4UniformRand() <= emissionProbability)
		return true;
	else
		return false;
}
