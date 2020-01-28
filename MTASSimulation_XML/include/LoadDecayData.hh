
#ifndef NEW_LDD_H
#define	NEW_LDD_H

#include <vector>
#include <string>
#include <cmath>

class Nuclide;
class Level;
class Transition;
struct Event;

class LoadDecayData
{
public:
	LoadDecayData();
    ~LoadDecayData();
    Nuclide LoadNuclideData(const std::string filename);
    void LoadDataFromXml();
	std::vector<Nuclide>* GetAllNuclides(){return &allNuclides_;}
	Level* GetStartLevel(){return startLevel_;}
	Level* GetStopLevel(){return stopLevel_;}


private:
	
	Level* startLevel_;
	Level* stopLevel_;
	std::vector<Nuclide> allNuclides_;
	
	void SetPointersToTransitions();
	void FindPointersToFinalLevels();
    Level* FindPointerToLevel(int atomicNumber, int atomicMass, double energy, double energyLvlUnc);
    void RecalculateIntensities(int atomicNumber, int atomicMass, double lvlEnergy,
	std::string transitionType, double transitionEnergy);

};

#endif	/* DECAY_H */
