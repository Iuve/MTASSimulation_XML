#ifndef MTASPulleys_h
#define MTASPulleys_h 1
#include "MTASMaterialsAndColorsManager.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class MTASPulleys
{
public:
	MTASPulleys(G4LogicalVolume* );
	~MTASPulleys();
	//G4LogicalVolume* GetBackboneLogVol(){return backboneLogVol;}

	
private:
	void SetParameters();
	void Construct();
	void LocateModules();
	void SetVisibleAtributes();
	
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* pulleyAMotherLogVol;
	G4LogicalVolume* pulleyBMotherLogVol;		
	
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double M_Pi;
	G4double m_inch;
	
};
	
#endif
