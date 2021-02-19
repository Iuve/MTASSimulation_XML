
#include "DeclareHeaders.hh"

#include <iostream>


Proton::Proton(std::string particleType, double transitionQValue, double intensity,
 double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber):
Transition(particleType, transitionQValue, intensity, finalLevelEnergy,
 finalLevelAtomicMass, finalLevelAtomicNumber)
{
 
}


Proton::~Proton()
{

}

