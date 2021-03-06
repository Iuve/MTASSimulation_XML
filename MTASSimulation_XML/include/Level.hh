
#ifndef NEW_LEVEL_H
#define	NEW_LEVEL_H

#include <vector>
#include <string>


class Level
{
public:
	Level(double levelEnergy, double spin, std::string parity, double halfLifeTimeInSeconds,
	 std::vector<Gamma> gammasFromLvL, std::vector<Beta> betasFromLvL,
	 std::vector<Neutron> neutronsFromLvL, std::vector<Alpha> alphasFromLvL,
	 std::vector<Proton> protonsFromLvL); 
	 
    Level(double levelEnergy, double spin, std::string parity, double halfLifeTimeInSeconds);

	~Level();
	
	double GetLevelEnergy(){return levelEnergy_;}
	double GetHalfLifeTime(){return halfLifeTimeInSeconds_;}
	
	std::vector<Beta>* GetBetaTransitions(){return &betasFromLvL_;}
	std::vector<Gamma>* GetGammaTransitions(){return &gammasFromLvL_;}
	std::vector<Neutron>* GetNeutronTransitions(){return &neutronsFromLvL_;}
	std::vector<Alpha>* GetAlphaTransitions(){return &alphasFromLvL_;}
	std::vector<Proton>* GetProtonTransitions(){return &protonsFromLvL_;}
	
    void EditLevelEnergy(double energy) {levelEnergy_ = energy; }
	void SetTransitions(std::vector<Transition*> transitions) { transitions_ = transitions; }
	std::vector<Transition*>* GetTransitions(){return &transitions_;}

    void RemoveTransition(std::string type, double energy);
    void UpdateTransitionVector();
    void AddTransition(std::string type, double transitionQValue, double intensity);

    void NormalizeTransitionIntensities();
	void CalculateTotalProbability();
	double GetTotalIntensity(){return totalIntensity_;}
    double GetSpin(){return spin_; }
    std::string GetParity(){return parity_; }

private:
	double levelEnergy_;
	double spin_;
	std::string parity_; // string or int -1, 1 ??
	double halfLifeTimeInSeconds_;
	
    std::vector<Gamma> gammasFromLvL_;
    std::vector<Beta> betasFromLvL_;
    std::vector<Neutron> neutronsFromLvL_;
    std::vector<Alpha> alphasFromLvL_;
    std::vector<Proton> protonsFromLvL_;
	
	std::vector<Transition*> transitions_;
	double totalIntensity_;
    std::vector<Transition*> transitionsToDelete_;
};

#endif	/* DECAY_H */
