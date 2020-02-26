
#include "DeclareHeaders.hh"


Transition::Transition(std::string particleType, double transitionQValue,
 double intensity, double finalLevelEnergy, int finalLevelAtomicMass,
 int finalLevelAtomicNumber, double electronConversionCoefficient, int atomicNumber):
particleType_(particleType), transitionQValue_(transitionQValue), intensity_(intensity),
 finalLevelEnergy_(finalLevelEnergy), finalLevelAtomicMass_(finalLevelAtomicMass),
 finalLevelAtomicNumber_(finalLevelAtomicNumber), electronConversionCoefficient_(electronConversionCoefficient),
 atomicNumber_(atomicNumber)
{

}


Transition::~Transition()
{

}


