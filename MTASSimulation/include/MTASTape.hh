#ifndef MTASTape_h
#define MTASTape_h 1
#include "MTASMaterialsAndColorsManager.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class MTASTape
{
public:
	MTASTape(G4LogicalVolume* );
	~MTASTape();
	
private:
	void SetParameters();
	void Construct();
	void SetVisibleAtributes();
	//void SetSensitiveDetector();
	
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* m_polyLayerLog;
	G4LogicalVolume* m_feLayerLog;
	G4LogicalVolume* m_scotchLayerLog;
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double m_inch; 
	G4double m_polyWidth;
	G4double m_polyThickness;
	G4double m_polyLength;
	
	G4double m_feWidth;
	G4double m_feThickness;
	G4double m_feLength;	
	
	G4double m_scotchWidth;
	G4double m_scotchThickness;
	G4double m_scotchLength;
};
	
#endif
