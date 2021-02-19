
#include "DeclareHeaders.hh"

#include "G4AtomicShells.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

#include <vector>
#include <iostream>


Beta::Beta(std::string particleType, double transitionQValue, double intensity,
 double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber):
Transition(particleType, transitionQValue, intensity, finalLevelEnergy,
 finalLevelAtomicMass, finalLevelAtomicNumber)
{
	if(GetParticleType() == "B-")
	{
		//beta minus
		betaEnergyDistribution_ = FermiDistribution(GetFinalLevelAtomicNumber() - 1, GetTransitionQValue(), -1);
	}
	else if(GetParticleType() == "B+")
	{
		//beta plus
		betaEnergyDistribution_ = FermiDistribution(GetFinalLevelAtomicNumber() + 1, GetTransitionQValue(), +1);
		//std::cout << "Beta+ created" << std::endl;
	}
	else if(GetParticleType() == "EC")
	{
		//electron capture
		//std::cout << "EC created" << std::endl;
	}
	else
		std::cout << "Wrong particle type in Beta class." << std::endl; 
		
}

Beta::~Beta()
{

}

void Beta::SetECCoef(std::string type, double value)
{
	//std::cout << type << " " << value << std::endl;
	if(type == "CK" || type == "CK+" )
		for(int i=0; i<4; i++)
			ECCoeff_[i] +=value;
	else if (type == "CL" || type == "CL+" )
		for(int i=1; i<4; i++)
			ECCoeff_[i] +=value;
	else if (type == "CM" || type == "CM+" )
		for(int i=2; i<4; i++)
			ECCoeff_[i] +=value;
	else
		ECCoeff_[3] +=value;
}

std::vector<Event> Beta::FindBetaEvent()
{
	std::vector<Event> betaEvents;
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

	if(GetParticleType() == "B-")
	{
		//beta minus
		double randomBetaEnergy = betaEnergyDistribution_.GetRandomBetaEnergy();
		betaEvents.push_back( Event(randomBetaEnergy, particleTable->FindParticle("e-")) );
	}
	else if(GetParticleType() == "B+")
	{
		//beta plus
		double randomBetaEnergy = betaEnergyDistribution_.GetRandomBetaEnergy();
		betaEvents.push_back( Event(randomBetaEnergy, particleTable->FindParticle("e+")) );	
	}
	else if(GetParticleType() == "EC") //EC
	{
		//std::cout << "EC simulation" << std::endl;
		atomicTransitionManager_ = G4AtomicTransitionManager::Instance();
		atomicTransitionManager_->Initialise();
			
		atomicNumber_ = GetFinalLevelAtomicNumber() + 1;
		int primaryVacancies = FindPrimaryVacancies();
		//Auger or X-rays
		double totalRadTransitProb = atomicTransitionManager_->
		TotalRadiativeTransitionProbability(atomicNumber_, primaryVacancies);

		//x rays
		if(G4UniformRand() < totalRadTransitProb)	
		{
			AddXRaysEvent(betaEvents, primaryVacancies);
			//std::cout << "X-ray" << std::endl;
		}
		//non rad - Auger
		else
		{
			AddXRaysEvent(betaEvents, primaryVacancies);
			//std::cout << "Auger" << std::endl;	
		}
	}
	else
		std::cout << "Wrong particle type in Beta class." << std::endl;
		
	return betaEvents;
}

int Beta::FindPrimaryVacancies()
{
	const unsigned int nrOfShells = atomicTransitionManager_-> NumberOfShells(atomicNumber_);
	double randomNumber = G4UniformRand()*ECCoeff_[3];
	if(randomNumber<=ECCoeff_[0])//K - shell nr = 0
		return 0;
		
	else if(randomNumber>ECCoeff_[0] && randomNumber<=ECCoeff_[1])// L - shell 1
		if(1>nrOfShells)
			return nrOfShells;
		else
			return 1;
		
	else if(randomNumber>ECCoeff_[1] && randomNumber<=ECCoeff_[2])// M - shell 4
		if(4>nrOfShells)
			return nrOfShells;
		else
			return 4;	
		
	else if(randomNumber>ECCoeff_[2] && randomNumber<=ECCoeff_[3])// N - shell 9
		if(9>nrOfShells)
			return nrOfShells;
		else
			return 9;
			
	else
	{
		std::cout << "Error in Beta::FindPrimaryVacancies()." << std::endl;
		return 0;
	}
}

void Beta::AddXRaysEvent(std::vector<Event> &decay, int primaryVacancies)
{
	const G4FluoTransition* fluoTrans = atomicTransitionManager_->
		ReachableShell(atomicNumber_, primaryVacancies);
	const G4DataVector transEnergies = fluoTrans -> TransitionEnergies();
	const G4DataVector transProb = fluoTrans -> TransitionProbabilities();
	int xRayIndex = FindRandomIndex(transProb);
	decay.push_back(Event(transEnergies.at(xRayIndex)/keV, G4ParticleTable::GetParticleTable()->FindParticle("gamma")));
}

void Beta::AddAugerEvent(std::vector<Event> &decay, int primaryVacancies)
{
	const G4AugerTransition * augerTrans = atomicTransitionManager_->
		ReachableAugerShell(atomicNumber_, primaryVacancies);

	//shell from where electron can fill the vacancy
	const std::vector<G4int> augerPossibilities = *(augerTrans->TransitionOriginatingShellIds());
	G4DataVector totAugerTransFromShell;
	for(unsigned int i=0; i<augerPossibilities.size(); i++)
	{
		const G4DataVector augerTransProb =
			 *(augerTrans -> AugerTransitionProbabilities(augerPossibilities.at(i)));
		double sumProbAugerTrans(0.);
		for(unsigned int j=0; j<augerTransProb.size(); j++)
			sumProbAugerTrans += augerTransProb.at(j);
		totAugerTransFromShell.push_back(sumProbAugerTrans);
	}
	int index = FindRandomIndex(totAugerTransFromShell);

	//electron from augerPossibilities.at(index) will fill the vacancy
	int electronShellId = augerPossibilities.at(index);
	const G4DataVector augerTransProb = *(augerTrans -> AugerTransitionProbabilities(electronShellId));
	const G4DataVector augerTransEnergy =  *(augerTrans -> AugerTransitionEnergies(electronShellId));
	int augerIndex = FindRandomIndex(augerTransProb);
	decay.push_back(Event(augerTransEnergy.at(augerIndex)/keV, G4ParticleTable::GetParticleTable()->FindParticle("e-")));
}

int Beta::FindRandomIndex( const G4DataVector transProb)
{
	double sumProb (0.);
	G4DataVector transProbSum;
	for (unsigned int i=0; i<transProb.size(); i++)
	{
		sumProb += transProb.at(i);
		transProbSum.push_back(sumProb);
	}
		
	double randomNumber = G4UniformRand()*sumProb;
	int index(-1);
	for (unsigned int i=0; i<transProb.size(); i++)
	{
		if(randomNumber < transProbSum.at(i))
		{
			index = i;
			break;
		}
	}
	return index;
}



