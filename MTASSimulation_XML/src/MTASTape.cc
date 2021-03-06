  /**
* Class constructing Aluminium Plates
*/

#include <math.h> 

#include "MTASTape.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

  
MTASTape::MTASTape(G4LogicalVolume* logicWorld)
{
	m_logicWorld=logicWorld;
	m_materialsAndColorsManager = MTASMaterialsAndColorsManager::GetInstance();
	SetParameters();
	Construct();
	SetVisibleAtributes();
}

MTASTape::~MTASTape()
{
}


void MTASTape::SetParameters()
{ 
	m_inch=2.54*cm; 
	m_polyWidth = 0.5 *m_inch;
	m_polyThickness = 0.035* mm;//0.06 *mm;
	m_polyLength = 60 *mm;
	
	m_feWidth = 0.5 *m_inch;
	m_feThickness = 0.012*mm;//0.015 *mm;
	m_feLength = 60 *mm;
	
	m_scotchWidth = m_polyWidth;
	//m_scotchThickness = 0.03 * mm;
	m_scotchThickness = 0.03 * mm;
	m_scotchLength = 20 * mm;
}  

void MTASTape::Construct()
{   

	G4Material* polyMaterial = m_materialsAndColorsManager->GetPolyethylene();
	G4Material* feMaterial = m_materialsAndColorsManager->GetIronOxide();
	G4Material* scotchMaterial = m_materialsAndColorsManager->GetPolyethylene();
	

	G4Box *polyLayerSolid = new G4Box ("polyLayerSolid",m_polyWidth/2.,m_polyThickness/2., m_polyLength/2.);
	m_polyLayerLog = new G4LogicalVolume( polyLayerSolid,  polyMaterial, "polyLogical");
	G4Box *feLayerSolid = new G4Box("feLayerSolid",m_feWidth/2.,m_feThickness/2., m_feLength/2.);
	m_feLayerLog = new G4LogicalVolume( feLayerSolid,  feMaterial, "feLogical");
	G4Box *scotchLayerSolid = new G4Box("scotchLayerSolid",m_scotchWidth/2.,m_scotchThickness/2., m_scotchLength/2.);
	m_scotchLayerLog = new G4LogicalVolume( scotchLayerSolid,  scotchMaterial, "scotchLogical");

	G4ThreeVector position( 0.0, m_feThickness+m_polyThickness+ m_scotchThickness/2., 0.0 );
	new G4PVPlacement( 0, position, m_scotchLayerLog, "scotchLayePhysical", m_logicWorld, 0, 0 );	
	position = G4ThreeVector( 0.0, m_feThickness+m_polyThickness/2., 0.0 );
	new G4PVPlacement( 0, position, m_polyLayerLog, "polyLayePhysical", m_logicWorld, 0, 0 );
  position = G4ThreeVector( 0.0, m_feThickness/2., 0.0 );
	new G4PVPlacement( 0, position, m_feLayerLog, "feLayePhysical", m_logicWorld, 0, 0 );
  position = G4ThreeVector( 0.0, -m_scotchThickness/2., 0.0 );
	new G4PVPlacement( 0, position, m_scotchLayerLog, "scotchLayePhysical", m_logicWorld, 0, 0 );	
	
	position = G4ThreeVector( 0.0, m_feThickness/2.-0.3 *mm, 0.0 );
	new G4PVPlacement( 0, position, m_polyLayerLog, "polyLayePhysical", m_logicWorld, 0, 0 );
  position = G4ThreeVector( 0.0, -m_polyThickness/2.-0.3 *mm, 0.0 );
	new G4PVPlacement( 0, position, m_feLayerLog, "feLayePhysical", m_logicWorld, 0, 0 );
	
}  
 
 
void MTASTape::SetVisibleAtributes() 
{
	m_polyLayerLog->SetVisAttributes(m_materialsAndColorsManager->GetYellow());//(m_Att2);
	m_feLayerLog->SetVisAttributes(m_materialsAndColorsManager->GetGrey());
	m_scotchLayerLog->SetVisAttributes(m_materialsAndColorsManager->GetRed());
} 
