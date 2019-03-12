#ifndef MTASPassiveShield_h
#define MTASPassiveShield_h 1


#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "MTASMaterialsAndColorsManager.hh"

class MTASPassiveShield
{
public:
	MTASPassiveShield(G4LogicalVolume* );
	~MTASPassiveShield();
	G4LogicalVolume* GetShield2_logVol(){return m_Shield2_logVol;}
	G4LogicalVolume* GetShield1_logVol(){return m_Shield1_logVol;}
	G4LogicalVolume* GetShield0_logVol(){return m_Shield0_logVol;}
	
	G4double GetShieldx(){return m_Shieldx;}
	G4double GetShieldy(){return m_Shieldy;}
	G4double GetShieldz(){return m_Shieldz;}
	
private:
	void SetParameters();
	void Construct();
	void SetVisibleAtributes();
	//void SetSensitiveDetector();
	
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* m_Shield2_logVol;
	G4LogicalVolume* m_Shield1_logVol;
	G4LogicalVolume* m_Shield0_logVol;
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	G4MultiFunctionalDetector* DetShield;
	
	G4double m_inch; 
	G4double M_Pi;
	G4double m_CristalSide;
	G4double m_RodsLength;
	G4double m_AlThinPlateDistance;
	G4double m_LiftingEyesLength;
	
	G4double m_Shield0Thickness;
	G4double m_Shield1Thickness;
	G4double m_Shield2Thickness;

	G4double m_Shieldx; //internal shield dimensions
	G4double m_Shieldy;
	G4double m_Shieldz; 
	
};
	
#endif
