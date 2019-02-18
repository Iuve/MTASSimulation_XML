

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
