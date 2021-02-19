
#ifndef NEW_PROTON_H
#define	NEW_PROTON_H

#include <string>


class Proton : public Transition
{
public:
	Proton(std::string particleType, double transitionQValue, double intensity,
	 double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber);
	 
	~Proton();
	
	
private:


};

#endif

