//
//* moja próba napisania konstrukcji detektora
#include <math.h> 
#include "MTASDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSNofSecondary.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"



MTASDetectorConstruction::MTASDetectorConstruction()
{
	m_ActiveShield=0;
	m_PassiveShield=0;
	// Świat
	m_logicWorld=0;
	m_physiWorld=0;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
}

MTASDetectorConstruction::~MTASDetectorConstruction()
{
	if(m_AlPlate != 0)
		delete m_AlPlate;
		
	if(m_PassiveShield != 0)
		delete m_PassiveShield;
		
	if(m_ActiveShield != 0)
		delete m_ActiveShield;
	
	if(m_HexagonModules != 0)
		delete m_HexagonModules;
		
//	if(m_PhysicalSource != 0)
//		delete m_PhysicalSource;
		
	if(m_SiliconDetectors != 0)
		delete m_SiliconDetectors;
		
	if(m_Ladder != 0)
		delete m_Ladder;
		
	if(m_Pulleys != 0)
		delete m_Pulleys;
		
	if(m_Tape != 0)
		delete m_Tape;
}


G4VPhysicalVolume* MTASDetectorConstruction::Construct()
{
	//World
	G4double HalfWorldLength=2*m;
	G4Box *solidWorld= new G4Box("world",HalfWorldLength,HalfWorldLength,HalfWorldLength);
	m_logicWorld= new G4LogicalVolume( solidWorld, m_materialsAndColorsManager->GetVaccum(), "World", 0, 0, 0);
	m_physiWorld = new G4PVPlacement(0,G4ThreeVector(),m_logicWorld,"World",0,false,0);

	m_AlPlate = new MTASAlPlate(m_logicWorld);
	//m_PassiveShield = new MTASPassiveShield(m_logicWorld);
	//m_ActiveShield = new MTASActiveShield(m_logicWorld);
	m_HexagonModules = new MTASHexagonModules(m_logicWorld);
	m_Ladder = new MTASLadder(m_logicWorld); //19.06.12 testy
	m_SiliconDetectors = new MTASSiliconDetectors (m_logicWorld);
	//m_PhysicalSource = new MTASPhysicalSource(m_logicWorld);
	m_Tape = new MTASTape(m_logicWorld);
	m_Pulleys = new MTASPulleys(m_logicWorld);//19.06.12 testy
  m_logicWorld->SetVisAttributes(m_materialsAndColorsManager->GetWhite());

	SetupDetectors();

	return m_physiWorld; 
}


void MTASDetectorConstruction::SetupDetectors()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(2);
 
 {
//   G4MultiFunctionalDetector* 
    DetCenter = new G4MultiFunctionalDetector("/NaICenter");
    G4VPrimitiveScorer* primitive;
    primitive = new G4PSEnergyDeposit("eDep",4);
    DetCenter->RegisterPrimitive(primitive);
    G4SDManager::GetSDMpointer()->AddNewDetector(DetCenter);
    G4LogicalVolume* NaI_Centre_logVol=m_HexagonModules->GetNaI_Centre_logVol();
    NaI_Centre_logVol->SetSensitiveDetector(DetCenter);
 }
 {  
//   G4MultiFunctionalDetector* 
   DetOuter = new G4MultiFunctionalDetector("/NaIOuter");
   G4VPrimitiveScorer* primitive1;
   primitive1 = new G4PSEnergyDeposit("eDep",4);
   DetOuter->RegisterPrimitive(primitive1);
   G4SDManager::GetSDMpointer()->AddNewDetector(DetOuter);
   G4LogicalVolume* NaI_logVol=m_HexagonModules->GetNaI_logVol();
   NaI_logVol->SetSensitiveDetector(DetOuter);
   
 } 
   {  
// SiliconDetectors 
   DetSili = new G4MultiFunctionalDetector("/Sili");
   G4VPrimitiveScorer* primitive;
   primitive = new G4PSEnergyDeposit("eDep",0);
   DetSili->RegisterPrimitive(primitive);
   G4SDManager::GetSDMpointer()->AddNewDetector(DetSili);
   G4LogicalVolume* m_siliStripLog=m_SiliconDetectors->GetSiliconLogVol();
   m_siliStripLog->SetSensitiveDetector(DetSili);
 }
   
       
}




