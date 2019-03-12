/*#ifndef MTASPhysicalSource_h
#define MTASPhysicalSource_h 1


#include "globals.hh"
#include "MTASMaterialsAndColorsManager.hh"
#include "G4LogicalVolume.hh"
class MTASPhysicalSource
{
public:
	MTASPhysicalSource(G4LogicalVolume* );
	~MTASPhysicalSource();
	G4LogicalVolume* GetSourceLogVol(){return m_sourceLogVol;}
	
private:
	void SetParameters();
	void Construct();
	void SetVisibleAtributes();
	//void SetSensitiveDetector();
	
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* m_sourceLogVol;
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double m_inch; 
	G4double M_Pi;
	G4double m_sourceRadius;
	G4double m_sourceHeight;
	
};
	
#endif
*/



#ifndef MTASPhysicalSource_h
#define MTASPhysicalSource_h 1


#include "globals.hh"
#include "MTASMaterialsAndColorsManager.hh"
#include "G4LogicalVolume.hh"
class MTASPhysicalSource
{
public:
	MTASPhysicalSource(G4LogicalVolume* );
	~MTASPhysicalSource();
	G4LogicalVolume* GetSourceLogVol(){return m_sourceLogVol;}
	
private:
	void SetParameters();
	void ConstructAlSource();
	void Construct90SrSource();
	void SetVisibleAtributes();
	//void SetSensitiveDetector();
	
	G4LogicalVolume* m_logicWorld;
	G4LogicalVolume* m_sourceLogVol;
	G4LogicalVolume* m_90SrRingLogVol;
	G4LogicalVolume* m_90SrInteriorLogVol;
	MTASMaterialsAndColorsManager* m_materialsAndColorsManager;
	
	G4double m_inch; 
	G4double M_Pi;
	G4double m_sourceRadius;
	G4double m_sourceHeight;
	G4double m_90SrRadius;
	G4double m_90SrInnerRadius;
	G4double m_90SrThickness;
	G4double m_90SrRingThickness;
	
};
	
#endif
