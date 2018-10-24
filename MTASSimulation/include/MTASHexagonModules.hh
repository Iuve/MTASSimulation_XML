/*
 * Class represent all NaI modules with 4-layers housing

*/
#ifndef MTASHexagonModules_h
#define MTASHexagonModules_h 1
#include "MTASMaterialsAndColorsManager.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class MTASHexagonModules
{
public:
	MTASHexagonModules(G4LogicalVolume* );
	~MTASHexagonModules();
	G4LogicalVolume* GetNaI_logVol(){return m_NaI_logVol;}
	G4LogicalVolume* GetNaI_Centre_logVol(){return m_NaI_Centre_logVol;}
	
private:
	void SetParameters();
	void Construct();
	void LocateModules();
	void SetVisibleAtributes();
	
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* m_layer0_logVol;
	G4LogicalVolume* m_layer1_logVol;
	G4LogicalVolume* m_layer2_logVol;
	G4LogicalVolume* m_layer3_logVol;
	G4LogicalVolume* m_NaI_logVol;
	G4LogicalVolume* m_layer0_Centre_logVol;
	G4LogicalVolume* m_layer1_Centre_logVol;
	G4LogicalVolume* m_layer2_Centre_logVol;
	G4LogicalVolume* m_layer3_Centre_logVol;
	G4LogicalVolume* m_NaI_Centre_logVol;
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double M_Pi;
	G4double m_inch;
	G4double m_NumberOfLayers;
	G4double m_CristalSide;
	G4double m_CristalLength;
	G4double m_Layer0Thickness;
	G4double m_Layer1Thickness;
	G4double m_Layer2Thickness;
	G4double m_Layer3Thickness;
	
	G4double m_Layer0InCenterHoleThickness;
	G4double m_Layer1InCenterHoleThickness;
	G4double m_Layer2InCenterHoleThickness;
	G4double m_Layer3InCenterHoleThickness;
	
	G4double m_PipeRadius;//Promień zerwnętrzny aluminiowej rury
	G4double m_PipeLength;
	G4double m_TubsRadius;
	
};
	
#endif
