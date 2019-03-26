/*
 * Saving data as a root file. It has 4 branches:
 * 1) EnDepInModulesPart1 - energy deposited in all Mtas, central module and inner ring
 * 2) EnDepInModulesPart2 - energy deposited in all Mtas, middle and outer ring
 * 3) LightPart1 - analogously to the energy
 * 4) LightPart1 
 * 5) EnDepInSili1 - energy deposited in upper silicon
 * 6) EnDepInSili2 - energy in lower sili
 */

#ifndef MTASAnalysisManger_h
#define MTASAnalysisManger_h 1

#include <stdio.h>
#include <vector>
#include <string>
#include "TFile.h"
#include "TNtuple.h"

#include "G4LogicalVolume.hh"
#include "globals.hh"

class MTASAnalysisManager
{
public:
	MTASAnalysisManager(std::string outputFile);
	virtual ~MTASAnalysisManager();

public:
	void AddEnergyDepositedInCell(const int eventID , const G4double totEnDep, const G4double *enDepInAllModules);
	void AddEnergyDepositedInCell2(const int eventID , const G4double totEnDep, const G4double *enDepInAllModules);
	void AddSiliconEnergy1(const int eventID , const G4double totSiliEn, const G4double *siliEn);
	void AddSiliconEnergy2(const int eventID , const G4double totSiliEn, const G4double *siliEn);
	void AddLight1(const int eventID , const G4double totEnDep, std::vector <G4double> lightProdInNaIMod);
	void AddLight2(const int eventID , const G4double totEnDep, std::vector <G4double> lightProdInNaIMod);

//unused - for all rings		
	void AddCellCounting( const int eventID , const G4double totalEnergyDepositedInEvent, const G4double EnergyDepositedInCentralRing,const G4double EnergyDepositedInMiddleRing,const G4double EnergyDepositedInExternalRing );

private:


	TFile* m_RootOutputFile;
	TNtuple* m_energyNTuple;
	TNtuple* m_energy2NTuple;
	TNtuple* m_siliNTuple1;
	TNtuple* m_siliNTuple2;
	TNtuple* m_lightNTuple1;
	TNtuple* m_lightNTuple2;

  void SetUp();
  void CleanUp();
  void MakeAndOpenOutputFile();
  void CloseOutputFile();
  
  G4int m_temp;
  G4bool m_NormalizeOutput;
  
  std::string m_outputFile;
};


#endif // MTASAnalysisManger_h


