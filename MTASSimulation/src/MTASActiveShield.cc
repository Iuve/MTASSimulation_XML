/**
* Class constructing ActiveShield
*/

#include <math.h> 

#include "MTASActiveShield.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"

#include "G4ios.hh"
using namespace CLHEP;
  
MTASActiveShield::MTASActiveShield(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	SetVisibleAtributes();
}

MTASActiveShield::~MTASActiveShield()
{
	//if(m_materialsAndColorsManager != 0)
		//delete m_materialsAndColorsManager;
}

void MTASActiveShield::SetParameters()
{ 
	m_inch=2.54*cm; 
	M_Pi=4*atan(1);
	m_CristalSide=4 *m_inch;
	m_RodsLength= 1249 *mm;
	m_AlThinPlateDistance = 0.5*m_RodsLength;
	m_LiftingEyesLength = 60 *mm;
	
	m_Shield0Thickness = 5 * cm;
	m_Shield1Thickness = 10 * cm;
	m_Shield2Thickness = 1 * cm;
	m_CristalSide = 4* m_inch;

	m_Shieldx = 3*m; //internal shield dimensions
	m_Shieldy = 3*m;
	m_Shieldz = 3*m;// distance between two thin al plate + lifting eyes
}  

void MTASActiveShield::Construct()
{   
	//G4Material* Shield0_Material = m_materialsAndColorsManager->GetAir();
	//G4Material* Shield1_Material = m_materialsAndColorsManager->GetParaffin();
	//G4Material* Shield2_Material = m_materialsAndColorsManager->GetBC408();
	G4Material* Shield0_Material = m_materialsAndColorsManager->GetAir();
	G4Material* Shield1_Material = m_materialsAndColorsManager->GetAir();
	G4Material* Shield2_Material = m_materialsAndColorsManager->GetAir();//simulation without shield
	
	
	const G4ThreeVector zero( 0.0, 0.0, 0.0 );
	
//Construct Shield
//Shield 0 (external)
	G4double Shield0x = m_Shieldx + 2* (m_Shield0Thickness + m_Shield1Thickness + m_Shield2Thickness);
	G4double Shield0y = m_Shieldy + 2* (m_Shield0Thickness + m_Shield1Thickness + m_Shield2Thickness);
	G4double Shield0z = m_Shieldz + 2* (m_Shield0Thickness + m_Shield1Thickness + m_Shield2Thickness);

	G4Box *Shield0_Solid = new G4Box("Shield0_Solid", Shield0x/2.0, Shield0y/2.0,  Shield0z/2.0);
	G4Box *Shield0Air_Solid = new G4Box("Shield0Air_Solid", (Shield0x-2*m_Shield0Thickness)/2.0, (Shield0y-2*m_Shield0Thickness)/2.0,  (Shield0z-2*m_Shield0Thickness)/2.0);
	G4SubtractionSolid * Shield0_Subtraction = new G4SubtractionSolid("Shield0_Subtraction",Shield0_Solid,Shield0Air_Solid);
	m_Shield0_logVol = new G4LogicalVolume( Shield0_Subtraction, Shield0_Material, "Shield0_Logical");
	new G4PVPlacement( 0, zero, m_Shield0_logVol, "Shield0_Physical", m_logicWorld, 0, 0 );
  
//Shield 1
	G4double Shield1x = Shield0x - 2* (m_Shield0Thickness);
	G4double Shield1y = Shield0y - 2* (m_Shield0Thickness);
	G4double Shield1z = Shield0z - 2* (m_Shield0Thickness);

	G4Box *Shield1_Solid = new G4Box("Shield1_Solid", Shield1x/2.0, Shield1y/2.0,  Shield1z/2.0);
	G4Box *Shield1Air_Solid = new G4Box("Shield1Air_Solid", (Shield1x-2*m_Shield1Thickness)/2.0, (Shield1y-2*m_Shield1Thickness)/2.0,  (Shield1z-2*m_Shield1Thickness)/2.0);
	G4SubtractionSolid * Shield1_Subtraction = new G4SubtractionSolid("Shield1_Subtraction",Shield1_Solid,Shield1Air_Solid);
	m_Shield1_logVol = new G4LogicalVolume( Shield1_Subtraction, Shield1_Material, "Shield1_Logical");
	new G4PVPlacement( 0, zero, m_Shield1_logVol, "Shield1_Physical", m_logicWorld, 0, 0 );
  
//Shield 2
	G4double Shield2x = Shield1x - 2* (m_Shield1Thickness);
	G4double Shield2y = Shield1y - 2* (m_Shield1Thickness);
	G4double Shield2z = Shield1z - 2* (m_Shield1Thickness);

	G4Box *Shield2_Solid = new G4Box("Shield2_Solid", Shield2x/2.0, Shield2y/2.0,  Shield2z/2.0);
	G4Box *Shield2Air_Solid = new G4Box("Shield2Air_Solid", (Shield2x-2*m_Shield2Thickness)/2.0, (Shield2y-2*m_Shield2Thickness)/2.0,  (Shield2z-2*m_Shield2Thickness)/2.0);
	G4SubtractionSolid * Shield2_Subtraction = new G4SubtractionSolid("Shield2_Subtraction",Shield2_Solid,Shield2Air_Solid);
	m_Shield2_logVol = new G4LogicalVolume( Shield2_Subtraction, Shield2_Material, "Shield2_Logical");
	new G4PVPlacement( 0, zero, m_Shield2_logVol, "Shield2_Physical", m_logicWorld, 0, 0 );
}  
 
 
void MTASActiveShield::SetVisibleAtributes() 
{
	m_Shield0_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGrey());//(m_Att1);
	m_Shield1_logVol->SetVisAttributes(m_materialsAndColorsManager->GetYellow());//(m_Att2);
	m_Shield2_logVol->SetVisAttributes(m_materialsAndColorsManager->GetGreen());//(m_Att0);
}  


