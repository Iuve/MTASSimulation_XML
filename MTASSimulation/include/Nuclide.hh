

#ifndef NEW_NUCLIDE_H
#define	NEW_NUCLIDE_H
#include "Level.hh"
#include <vector>
#include <map>


class Level;
class Nuclide
{
public:
	Nuclide(int atomicNumber, int atomicMass, std::vector<Level> nuclideLevels);
	~Nuclide();
	int GetAtomicMass(){return atomicMass_;}
	int GetAtomicNumber(){return atomicNumber_;}
	std::vector<Level>* GetNuclideLevels(){return &nuclideLevels_;}


private:
	int atomicNumber_;
	int atomicMass_;
	std::vector<Level> nuclideLevels_;
	

};

#endif	/* DECAY_H */
