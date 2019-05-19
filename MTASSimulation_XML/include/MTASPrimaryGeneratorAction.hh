
#ifndef MTASPrimaryGeneratorAction_h
#define MTASPrimaryGeneratorAction_h 1

#include "Decay.hh"
#include "MTASDetectorConstruction.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include <vector>

static bool g_decaySimulationStatus(false);


class MTASPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
     MTASPrimaryGeneratorAction(MTASDetectorConstruction*);
    ~MTASPrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* anEvent);

  private:
  
  //testy
    G4ParticleGun* m_ParticleGun;
    MTASDetectorConstruction* m_MTASDetectorConstruction;
    Decay* decay_;
    //vector<double> m_BcgEnergy;
    //vector<double> m_BcgCount;
    //G4double* m_MaxCount;
    //G4int m_NoOfEleInFile;
		
	void SetUp( void );
	void LoadBackgroundFile();
	
	 //position
	void GenerateSmoothCenterStartPosition(G4ThreeVector*, G4double=0., G4double=0.);
	void GenerateBackgroundStartPosition(G4ThreeVector*);
   
	//direction
	void  GenerateIsotropicDirectionDistribution(G4ThreeVector*, G4double);
	void  GenerateBackgroundDirectionDistribution(G4ThreeVector*, G4ThreeVector);
   
	//energy
	G4double GenerateBackgroundEnergyDistribution();
	
	void GenerateDecay( G4Event* anEvent );
	void GenerateSingleParticle( G4Event* anEvent );
	
	
};

#endif // MTASPrimaryGeneratorAction_h
