

#include "Nuclide.hh"

#include <vector>




Nuclide::Nuclide(int atomicNumber, int atomicMass, std::vector<Level> nuclideLevels):
atomicNumber_(atomicNumber), atomicMass_(atomicMass), nuclideLevels_(nuclideLevels)
{
	
}

Nuclide::~Nuclide()
{
	
}
