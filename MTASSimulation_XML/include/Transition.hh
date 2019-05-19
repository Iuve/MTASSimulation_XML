

#ifndef NEW_TRANSITION_H
#define	NEW_TRANSITION_H
#include "Level.hh"
#include "Decay.hh"
#include "FermiDistribution.hh"
#include "G4DataVector.hh"
#include "G4AtomicTransitionManager.hh"
#include <string>
#include <vector>


class Level;
struct Event;
class Transition
{
public:
	//static const int numberOfShellIndexes_ = 4;
	//Transition(std::string type, double transitionQValue, double intensity, double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber);
	Transition(std::string type, double transitionQValue, double intensity, double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber, 
	FermiDistribution* betaEnergyDistribution, double electronConversionCoefficient = 0.);
	Transition(std::string type, double transitionQValue, double intensity, double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber,
	int atomicNumber, double electronConversionCoefficient = 0.);
	
	~Transition();
	
	double GetFinalLevelEnergy(){return finalLevelEnergy_;}
	int GetFinalLevelAtomicMass(){return finalLevelAtomicMass_;}
	int GetFinalLevelAtomicNumber(){return finalLevelAtomicNumber_;}
	double GetTransitionQValue(){return transitionQValue_;}
	std::string GetParticleType(){return particleType_;}
	
	double GetIntensity(){return intensity_;}
	void SetRunningIntensity(double runningIntensity){runningIntensity_ = runningIntensity;} // intensities for calculations
	double GetRunningIntensity(){return runningIntensity_;}

	void SetPointerToFinalLevel(Level* pointerToFinalLevel){pointerToFinalLevel_ = pointerToFinalLevel;}
	Level* GetPointerToFinalLevel(){return pointerToFinalLevel_;}
	// for beta purposes
	FermiDistribution* GetBetaEnergyDistribution(){return betaEnergyDistribution_;}
	std::vector<Event> FindBetaPlusEvents();
	// for gamma purposes
	double GetElectronConversionCoefficient(){return electronConversionCoefficient_;}
	void FindICEvent(std::vector<Event> &decay);
	void SetShellElectronConvCoef(std::string type, double value);
	std::vector<Event> FindGammaEvents();
	

private:
	std::string particleType_;
	double transitionQValue_;
	Level* pointerToFinalLevel_;
	double finalLevelEnergy_;
	int finalLevelAtomicMass_;
	int finalLevelAtomicNumber_;
	double intensity_;
	double runningIntensity_;
	
	//for beta purposes
	FermiDistribution* betaEnergyDistribution_;
	bool IsBetaPlusDecay();
	
	//for gamma purposes
	static const int numberOfShellIndexes_ = 4;
	void InitializeShellNumbers();
	int FindPrimaryShellIndex();
	bool IsRadiativeTransitionReachableShell(int shellIndex);
	bool IsAugerReachableShell(int shellIndex);
	void AddXRaysEvent(std::vector<Event> &decay, int primaryVacancies);
	void AddAugerEvent(std::vector<Event> &decay, int primaryVacancies);
	int FindRandomIndex( const G4DataVector transProb);
	bool IsGammaDecay();
	int atomicNumber_;
	
	
	double shellElectonConvCoeff_[numberOfShellIndexes_];
	double electronConversionCoefficient_;
	int shellNumbers_[numberOfShellIndexes_]; 
	G4AtomicTransitionManager* atomicTransitionManager_;

};
/*
class Gamma: public Transition
{
public:
	Gamma(std::string type, double transitionQValue, double intensity, double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber,
	double electronConversionCoefficient = 0.);

	Gamma(std::string type, double transitionQValue, double intensity, double finalLevelEnergy, int finalLevelAtomicMass, int finalLevelAtomicNumber,
	double electronConversionCoefficient, double shellElectonConvCoeff[]);
	
	//double GetElectronConversionCoefficient(){return electronConversionCoefficient_;}
	void FindICEvent(std::vector<Event> &decay);
	
private:
	void InitializeShellNumbers();
	int FindPrimaryShellIndex();
	bool IsRadiativeTransitionReachableShell(int shellIndex);
	bool IsAugerReachableShell(int shellIndex);
	void AddXRaysEvent(std::vector<Event> &decay, int primaryVacancies);
	void AddAugerEvent(std::vector<Event> &decay, int primaryVacancies);
	int FindRandomIndex( const G4DataVector transProb);
	
	static const int numberOfShellIndexes_ = 4;
	double shellElectonConvCoeff_[numberOfShellIndexes_];
	double electronConversionCoefficient_;
	int shellNumbers_[numberOfShellIndexes_]; 
	G4AtomicTransitionManager* atomicTransitionManager_;
}; */

#endif	/* DECAY_H */
