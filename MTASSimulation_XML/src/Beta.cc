
#include "Beta.hh"
#include "Decay.hh"
#include "G4ParticleTable.hh"

#include <vector>
#include <string>
#include <iostream>



Beta::Beta(double maxEnergy, int eCharge, FermiDistribution* betaEnergyDistribution):
maxBetaEnergy_(maxEnergy), eCharge_(eCharge), betaEnergyDistribution_(betaEnergyDistribution)
{
	FindBetaEvent();
}


Beta::~Beta()
{

}


void Beta::FindBetaEvent()
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	double randomBetaEnergy = betaEnergyDistribution_->GetRandomBetaEnergy();
	if(eCharge_ < 0)
	{
		//beta minus
		betaEvents_.push_back( Event(randomBetaEnergy, particleTable->FindParticle("e-")) );
	}
	else 
	{
		//beta plus
		betaEvents_.push_back( Event(randomBetaEnergy, particleTable->FindParticle("e+")) );	
	}
}
