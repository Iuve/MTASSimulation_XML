#ifndef MTASLadder_h
#define MTASLadder_h 1
#include "MTASMaterialsAndColorsManager.hh"

#include "globals.hh"

class MTASLadder
{
public:
	MTASLadder(G4LogicalVolume* );
	~MTASLadder();
	//G4LogicalVolume* GetBackboneLogVol(){return backboneLogVol;}

	
private:
	void SetParameters();
	void Construct();
	void LocateModules();
	void SetVisibleAtributes();
	
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* backboneLogVol;
	//G4LogicalVolume* backboneLogVol2;
	//G4LogicalVolume* backboneLogVol3;
	G4LogicalVolume* pipeLogVol;
	G4LogicalVolume* cableLogVol; //MS October 2022
        G4LogicalVolume* squareCableLogVol;
	
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double M_Pi;
	G4double m_inch;

	G4double a;
	G4double b;
	G4double c;
	G4double A;
	G4double B;
	G4double C;
	G4double length;
	G4double ladderXPosition;
	
	G4double notchA;
	G4double notchLength;
	G4double notchZPosition;
	
	G4double pipeOuterRadius;
	G4double pipeThickness;
	G4double pipeLength;
	
};
	
#endif
