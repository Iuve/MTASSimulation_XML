
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
    LoadDecayData(std::string);
    ~LoadDecayData();
    Nuclide LoadNuclideData(const std::string filename);
    void LoadDataFromXml();
	std::vector<Nuclide>* GetAllNuclides(){return &allNuclides_;}
	Level* GetStartLevel(){return startLevel_;}
	Level* GetStopLevel(){return stopLevel_;}
	
	double GetCycleDurationInSeconds() { return cycleDurationInSeconds_; }
	double GetEventDurationInSeconds() { return eventDurationInSeconds_; }
	double GetMeasureDelayInSeconds() { return measureDelayInSeconds_; }

    void RecalculatePointersToLevels();


private:
	
	Level* startLevel_;
	Level* stopLevel_;
	std::vector<Nuclide> allNuclides_; //is it protected from overwriting???
	
    void FindPointersToFinalLevels();
    void FindPointersToInitialLevels();
	void SetPointersToTransitions();
    Level* FindPointerToLevel(int atomicNumber, int atomicMass, double energy, double energyLvlUnc);
    void RecalculateIntensities(int atomicNumber, int atomicMass, double lvlEnergy,
	std::string transitionType, double transitionEnergy);
	
	double cycleDurationInSeconds_;
	double eventDurationInSeconds_;
	double measureDelayInSeconds_;

};

#endif	/* DECAY_H */
