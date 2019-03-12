#ifndef MTASSiliconDetectors_h
#define MTASSiliconDetectors_h 1
#include "MTASMaterialsAndColorsManager.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class MTASSiliconDetectors
{
public:
	MTASSiliconDetectors(G4LogicalVolume* );
	~MTASSiliconDetectors();
	G4LogicalVolume* GetSiliconLogVol(){return siliStripLog;}
	
private:
	void SetParameters();
	void Construct();
	void LocateModules();
	void SetVisibleAtributes();
	void ConstructSilicon();
	void ConstructFrame();
	G4LogicalVolume *m_logicWorld;
	G4LogicalVolume *siliPanelLogUp;
	G4LogicalVolume *siliPanelLogDown;
	G4LogicalVolume *siliStripLog; 
	G4LogicalVolume *connectorLog;
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double M_Pi;
	G4double m_inch;

	G4double m_panelLength;
	G4double m_panelWidth;
	G4double m_panelThickness;
	G4double m_activeAreaLendth;
	G4double m_activeAreaWidth;
	G4double m_stripSeparation;
	G4double m_nrOfStrips;
	G4double m_deadLayerThickness;
	
	//connector panel sizes:
	G4double m_connPanA;
	G4double m_connPanB;
	G4double m_connPanC;
	G4double m_connPanD;
	G4double m_connPanZ;
	G4double m_siliconPositionY;
	G4double m_framePositionY;
};
	
#endif
