

#ifndef MTASDetectorConstruction_h
#define MTASDetectorConstruction_h 1


#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4MultiFunctionalDetector.hh"

#include "MTASMaterialsAndColorsManager.hh"
#include "MTASAlPlate.hh"
#include "MTASPassiveShield.hh"
#include "MTASActiveShield.hh"
#include "MTASHexagonModules.hh"
#include "MTASPhysicalSource.hh"
#include "MTASLadder.hh"
#include "MTASPulleys.hh"
#include "MTASTape.hh"
#include "MTASSiliconDetectors.hh"


class MTASDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MTASDetectorConstruction();
	~MTASDetectorConstruction();
	
	G4VPhysicalVolume* Construct();
	G4LogicalVolume* GetWorldLogicalVolume() const { return m_logicWorld; }
	
	MTASPassiveShield* GetPassiveShield() {return m_PassiveShield;}
	MTASActiveShield* GetActiveShield() {return m_ActiveShield;}

private:
	void SetupDetectors();

	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	MTASAlPlate* m_AlPlate;
	MTASPassiveShield* m_PassiveShield; 
	MTASActiveShield* m_ActiveShield;
	MTASHexagonModules* m_HexagonModules;
	MTASLadder* m_Ladder;
	MTASPulleys* m_Pulleys;
	MTASSiliconDetectors* m_SiliconDetectors;
	MTASTape* m_Tape;

	MTASPhysicalSource* m_PhysicalSource;
// World

	G4VPhysicalVolume* m_physiWorld;
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* m_ModuleTotal_LogicalVolumes;
	G4LogicalVolume* m_NaI_Centre_logVol;
	G4LogicalVolume* m_NaI_logVol;
	G4LogicalVolume* m_siliStripLog;
	//G4LogicalVolume* m_Shield_LogVol;

	
	G4int verboseLevel;
	

	G4MultiFunctionalDetector* DetCenter;
	G4MultiFunctionalDetector* DetOuter;
	//G4MultiFunctionalDetector* DetShield;
	G4MultiFunctionalDetector* DetSili;

};

#endif 

