

#ifndef NEW_LDD_H
#define	NEW_LDD_H
#include "Nuclide.hh"
#include "Decay.hh"
#include <vector>
#include <string>

class Nuclide;
class Level;

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
	
	//static const int numberOfShellIndexes_ = 4;
	//void SetShellElectronConvCoef(std::string type, double value, double shellElectonConvCoeff_[]);
	
	Level* startLevel_;
	Level* stopLevel_;
	std::vector<Nuclide> allNuclides_;
	void FindPointersToFinalLevels();
    Level* FindPointerToLevel(int atomicNumber, int atomicMass, double energy, double energyLvlUnc);

};

#endif	/* DECAY_H */
