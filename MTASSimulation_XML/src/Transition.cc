
#include "Transition.hh"
#include "FermiDistribution.hh"

#include "Decay.hh"
#include "Exception.hh"
#include "globals.hh"
#include "G4AtomicShells.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include <assert.h>
#include <iostream>
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"


Transition::Transition(std::string particleType, double transitionQValue, double intensity, double finalLevelEnergy, int finalLevelAtomicMass, 
int finalLevelAtomicNumber, int atomicNumber, double electronConversionCoefficient):
particleType_(particleType), transitionQValue_(transitionQValue), intensity_(intensity), finalLevelEnergy_(finalLevelEnergy), 
finalLevelAtomicMass_(finalLevelAtomicMass), finalLevelAtomicNumber_(finalLevelAtomicNumber), 
atomicNumber_(atomicNumber), electronConversionCoefficient_(electronConversionCoefficient)
{
	atomicTransitionManager_ = G4AtomicTransitionManager::Instance();
	atomicTransitionManager_->Initialise();
	InitializeShellNumbers();
}

Transition::Transition(std::string particleType, double transitionQValue, double intensity, double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber, FermiDistribution* betaEnergyDistribution):
particleType_(particleType), transitionQValue_(transitionQValue), intensity_(intensity), finalLevelEnergy_(finalLevelEnergy), finalLevelAtomicMass_(finalLevelAtomicMass), finalLevelAtomicNumber_(finalLevelAtomicNumber), betaEnergyDistribution_(betaEnergyDistribution)
{
	
}

Transition::~Transition()
{

}

void Transition::InitializeShellNumbers() 
{
	shellNumbers_[0] = 0;
	shellNumbers_[1] = 1;
	shellNumbers_[2] = 4;
	shellNumbers_[3] = 9;
}

void Transition::SetShellElectronConvCoef(string type, double value)
{
    if(type == "KC" || type == "KC+" )
			for(int i=0; i<numberOfShellIndexes_; i++)
	    	shellElectonConvCoeff_[i] +=value;
    else if (type == "LC" || type == "LC+" )
			for(int i=1; i<numberOfShellIndexes_; i++)
	    	shellElectonConvCoeff_[i] +=value;
    else if (type == "MC" || type == "MC+" )
			for(int i=2; i<numberOfShellIndexes_; i++)
	    	shellElectonConvCoeff_[i] +=value;
    else
			shellElectonConvCoeff_[3] +=value;
}

std::vector<Event> Transition::FindGammaEvents()
{
	std::vector<Event> gammaDecay;
	if(IsGammaDecay())
	{
		gammaDecay.push_back(Event(transitionQValue_, G4ParticleTable::GetParticleTable()->FindParticle("gamma")));	
	}
	else //IC
	{
		FindICEvent(gammaDecay);
	}
	return gammaDecay;
}

bool Transition::IsGammaDecay()
{
    double randomNumber = G4UniformRand() * intensity_ * (1+electronConversionCoefficient_);
    if(randomNumber > intensity_ * electronConversionCoefficient_ )
		return true;
    return false;	
}

void Transition::FindICEvent(std::vector<Event> &gammaDecay)
{
	int primaryShellIndex = FindPrimaryShellIndex();
	int primaryVacancies = shellNumbers_[primaryShellIndex];
//	std::cout << "primaryVacancies: " << primaryVacancies << endl;
	if(!IsRadiativeTransitionReachableShell(primaryVacancies))
		if(!IsAugerReachableShell(primaryVacancies))
			return FindICEvent(gammaDecay);

	if(IsRadiativeTransitionReachableShell(primaryVacancies))
	{
//		std::cout << "Radiative Transition Reachable" << endl;
		if(IsAugerReachableShell(primaryVacancies))
		{
//			std::cout << "RTR: Auger Reachable" << endl;
			double totalRadTransitProb = atomicTransitionManager_->
				TotalRadiativeTransitionProbability(atomicNumber_, primaryVacancies);
			if(G4UniformRand()<totalRadTransitProb)
			{
//				std::cout << "RTR: Auger Reachable: Add X" << endl;
				AddXRaysEvent(gammaDecay, primaryVacancies);
			}
			else
			{
//				std::cout << "RTR: Auger Reachable: Add Auger" << endl;
				AddAugerEvent(gammaDecay, primaryVacancies);
			}	
		}
		else
		{
//			std::cout << "RTR: Auger NOT Reachable" << endl;
			AddXRaysEvent(gammaDecay, primaryVacancies);
		}
	}
	else //if(IsAugerReachableShell(primaryVacancies))
	{
//		std::cout << "Radiative Transition NOT Reachable" << endl;
		AddAugerEvent(gammaDecay, primaryVacancies);
	}

	double primaryBindingEnergy = 
	G4AtomicShells::GetBindingEnergy(atomicNumber_, primaryVacancies) / keV;
	gammaDecay.push_back(Event(transitionQValue_-primaryBindingEnergy, G4ParticleTable::GetParticleTable()->FindParticle("e-")));
}

int Transition::FindPrimaryShellIndex()
{	
	double randomNumber = G4UniformRand()* shellElectonConvCoeff_[3];
	if(randomNumber <= shellElectonConvCoeff_[0])//K - shell nr = 0
			return 0;
	for(int i=1; i < numberOfShellIndexes_; i++)
	{
		if(randomNumber > shellElectonConvCoeff_[i-1] && randomNumber <= shellElectonConvCoeff_[i])
			return i;
	}
	return 0;		
}

bool Transition::IsRadiativeTransitionReachableShell(int shellIndex)
{
	int numberOfRadiativeTransReachableShells = 
		atomicTransitionManager_->NumberOfReachableShells(atomicNumber_);
//	std::cout << "numberOfRadiativeTransReachableShells: " << numberOfRadiativeTransReachableShells << " shell index: " << shellIndex << endl;
	if(shellIndex >= numberOfRadiativeTransReachableShells)
		return false;
	else
		return true;
}

bool Transition::IsAugerReachableShell(int shellIndex)
{
	int numberOfAugerReachableShells = 
		atomicTransitionManager_->NumberOfReachableAugerShells(atomicNumber_);
	if(shellIndex >= numberOfAugerReachableShells)
		return false;
	else
		return true;
}

void Transition::AddXRaysEvent(std::vector<Event> &decay, int primaryVacancies)
{
	const G4FluoTransition* fluoTrans = atomicTransitionManager_->
		ReachableShell(atomicNumber_, primaryVacancies);
	const G4DataVector transEnergies = fluoTrans -> TransitionEnergies();
	const G4DataVector transProb = fluoTrans -> TransitionProbabilities();
	int xRayIndex = FindRandomIndex(transProb);
	decay.push_back(Event(transEnergies.at(xRayIndex)/keV, G4ParticleTable::GetParticleTable()->FindParticle("gamma")));
}

void Transition::AddAugerEvent(std::vector<Event> &decay, int primaryVacancies)
{
	const G4AugerTransition * augerTrans = atomicTransitionManager_->
		ReachableAugerShell(atomicNumber_, primaryVacancies);

	//shell from where electron can fill the vacancy
	const vector<G4int> augerPossibilities = *(augerTrans->TransitionOriginatingShellIds());
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

int Transition::FindRandomIndex( const G4DataVector transProb)
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

