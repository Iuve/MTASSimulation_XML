
#include "DeclareHeaders.hh"

#include <vector>
#include "Randomize.hh"

Level::Level(double levelEnergy, double spin, std::string parity, double halfLifeTimeInSeconds,
 std::vector<Gamma> gammasFromLvL, std::vector<Beta> betasFromLvL,
 std::vector<Neutron> neutronsFromLvL, std::vector<Alpha> alphasFromLvL):
levelEnergy_(levelEnergy), spin_(spin), parity_(parity), halfLifeTimeInSeconds_(halfLifeTimeInSeconds),
 gammasFromLvL_(gammasFromLvL), betasFromLvL_(betasFromLvL), neutronsFromLvL_(neutronsFromLvL),
 alphasFromLvL_(alphasFromLvL)
{

}


Level::~Level()
{

}

void Level::CalculateTotalProbability()
{
	totalIntensity_ = 0.;
	for ( auto it = transitions_.begin(); it != transitions_.end(); ++it  )
	{
		totalIntensity_ += (*it)->GetIntensity();
		(*it)->SetRunningIntensity(totalIntensity_);
	}
	std::cout << "Total Probability: " << totalIntensity_ << std::endl;
	std::cout << "Level energy: " << levelEnergy_ << std::endl;

}
