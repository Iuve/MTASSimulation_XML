/* 
 * File:   MTASAlPlate.hh
 * Author: aleksandra
 *
 * Front and back aluminium plates with round holes
 */
#ifndef MTASAlPlate_h
#define MTASAlPlate_h 1
#include "MTASMaterialsAndColorsManager.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class MTASAlPlate
{
public:
	MTASAlPlate(G4LogicalVolume* );
	~MTASAlPlate();
	
private:
	void SetParameters();
	void Construct();
	void SetVisibleAtributes();
	//void SetSensitiveDetector();

	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* m_AlThickPlate_logVol;
	G4LogicalVolume* m_Rod_logVol;
	G4LogicalVolume* m_AlThinPlate_logVol;
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double m_inch; 
	G4double M_Pi;
	
	G4double m_CristalSide;
	G4double m_AllLayersThickness;
	G4double m_AlThickPlateRadius; //closer NaI Radius- distance from the centre to the side
	G4double m_AlThickPlateThickness;
	G4double m_AlThickPlateDistance;
	G4double m_HoleInThickPlateRadius;
	G4double m_LiftingEyesLength;
	G4double m_RodsLength;
	G4double m_AlThinPlateRadius; //farther NaI
	G4double m_AlThinPlateThickness;
	G4double m_AlThinPlateDistance;
	G4double m_HoleInThinPlateRadius;
	G4double m_CentralHoleInThinPlateRadius;
	G4double m_HoleToRodRadius;
	G4double m_RodLocateRadius;


};
	
#endif
