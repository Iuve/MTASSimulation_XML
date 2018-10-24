  /**
* Class constructing Aluminium Plates
*/
/*
#include <math.h> 

#include "MTASPhysicalSource.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

  
MTASPhysicalSource::MTASPhysicalSource(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	SetVisibleAtributes();
}

MTASPhysicalSource::~MTASPhysicalSource()
{
}


void MTASPhysicalSource::SetParameters()
{ 
	m_inch=2.54*cm; 
	M_Pi=4*atan(1);
	//m_sourceRadius = 0.5002 *m_inch;
	//m_sourceHeight = 0.12 *m_inch;
	m_sourceRadius = 0.3 *m_inch;
	m_sourceHeight = 0.16 *mm;
}  

void MTASPhysicalSource::Construct()
{   

	G4Material* sourceMaterial = m_materialsAndColorsManager->GetAluminium();//?????????????

	const G4ThreeVector zero( 0.0, 0.0, 0.0 );

	G4Tubs *sourceSolid = new G4Tubs("sourceSolid", 0., m_sourceRadius,  m_sourceHeight/2.0, 0., 360.);
	m_sourceLogVol = new G4LogicalVolume( sourceSolid,  sourceMaterial, "sourceLogical");
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(pi/2.*rad);
	new G4PVPlacement( rot, zero, m_sourceLogVol, "sourcePhysical", m_logicWorld, 0, 0 );
  
}  
 
 
void MTASPhysicalSource::SetVisibleAtributes() 
{
	m_sourceLogVol->SetVisAttributes(m_materialsAndColorsManager->GetYellow());//(m_Att2);
} 






*/



  /**
* Class constructing Aluminium Plates
*/

#include <math.h> 

#include "MTASPhysicalSource.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4UnitsTable.hh"
using namespace CLHEP;

  
MTASPhysicalSource::MTASPhysicalSource(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct90SrSource();
	SetVisibleAtributes();
}

MTASPhysicalSource::~MTASPhysicalSource()
{
}


void MTASPhysicalSource::SetParameters()
{ 
	m_inch=2.54*cm; 
	M_Pi=4*atan(1);
	//m_sourceRadius = 0.5002 *m_inch;
	//m_sourceHeight = 0.12 *m_inch;
	m_sourceRadius = 0.3 *m_inch;
	m_sourceHeight = 0.16 *mm;
	
	m_90SrRadius = 0.5/2. *m_inch;
	m_90SrInnerRadius = 0.4/2. *m_inch;
	m_90SrThickness = 0.03 *mm;
	
	m_90SrRingThickness = 3 *mm;
}  

void MTASPhysicalSource::ConstructAlSource()
{   

	G4Material* sourceMaterial = m_materialsAndColorsManager->GetAluminium();//?????????????

	const G4ThreeVector zero( 0.0, 0.0, 0.0 );

	G4Tubs *sourceSolid = new G4Tubs("sourceSolid", 0., m_sourceRadius,  m_sourceHeight/2.0, 0., 360.);
	m_sourceLogVol = new G4LogicalVolume( sourceSolid,  sourceMaterial, "sourceLogical");
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(pi/2.*rad);
	new G4PVPlacement( rot, zero, m_sourceLogVol, "sourcePhysical", m_logicWorld, 0, 0 );
	
}  
 
void MTASPhysicalSource::Construct90SrSource()
{
	G4Material* ringMaterial = m_materialsAndColorsManager->GetAluminium();
	G4Material* interiorMaterial = m_materialsAndColorsManager->GetPolyethylene();
	//G4Material* interiorMaterial = m_materialsAndColorsManager->GetAluminium();
	
	G4Tubs *ringSolid = new G4Tubs("ringSolid", m_90SrInnerRadius, m_90SrRadius,
	m_90SrRingThickness, 0., 360.);
	m_90SrRingLogVol = new G4LogicalVolume(ringSolid, ringMaterial, "ringLog");
	
	const G4ThreeVector zero( 0.0, 0.0, 0.0 );
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(pi/2.*rad);
	new G4PVPlacement( rot, zero, m_90SrRingLogVol, "90SrRingPhysical",
	 m_logicWorld, 0, 0 );	
	 
	G4Tubs *interiorSolid = new G4Tubs("interiorSolid", 0., m_90SrInnerRadius,
	m_90SrThickness/2.0, 0., 360.);
	m_90SrInteriorLogVol = new G4LogicalVolume(interiorSolid,
	 interiorMaterial, "interiorLog");
	new G4PVPlacement( rot, zero, m_90SrInteriorLogVol, "90SrInteriorPhysical",
	 m_logicWorld, 0, 0 );	 
}
 
void MTASPhysicalSource::SetVisibleAtributes() 
{
	//m_sourceLogVol->SetVisAttributes(m_materialsAndColorsManager->GetYellow());//(m_Att2);
	m_90SrRingLogVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());
	m_90SrInteriorLogVol->SetVisAttributes(m_materialsAndColorsManager->GetYellow());
} 

